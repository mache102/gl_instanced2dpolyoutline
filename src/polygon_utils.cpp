#include "polygon_utils.h"

const std::vector<float> roundedCornerDirections = {
  5 * M_PI / 4,
  3 * M_PI / 4,
  1 * M_PI / 4,
  5 * M_PI / 4,
  1 * M_PI / 4,
  7 * M_PI / 4
};

/*
outline rounded corners for a polygon (inputted as an array of vertices)

v: original vertex
1,2,3,4: corner vertices (after applying directions in vertex shader) (5, 3, 1, 7)

  2----+----3
  | \     / |
  |   \ /   |
  +    v    +
  |   / \   |
  | /     \ |
  1----+----4

*/
std::pair<std::vector<float>, std::vector<float>> outlineRoundedCorners(std::vector<glm::vec2> vertices) {
  std::vector<float> cornerVertices = {};
  std::vector<float> cornerDirections = {};

  auto addCorner = [&](glm::vec2 vertex) {
    for (size_t i = 0; i < 6; i++) { 
      cornerVertices.push_back(vertex.x);
      cornerVertices.push_back(vertex.y);
    }
  };

  auto addDirections = [&] {
    cornerDirections.insert(cornerDirections.end(), roundedCornerDirections.begin(), roundedCornerDirections.end());
  };

  for (size_t i = 0; i < vertices.size(); i++) {
    addCorner(vertices[i]);
    addDirections();
  }

  return { cornerVertices, cornerDirections };
}


/*
outlines for a polygon (inputted as an array of vertices)

vertices: v1, v2, ..., vn
line impl vertices: v1,v2, v1,v2, | v2,v3, v2,v3, | ..., vn-1,vn, vn-1,vn, | vn,v1, vn,v1

1, 2: v1, v2
+, -: 1, 0 as a bit in attribute
two triangles:
  a. 1-, 1+, 2+
  b. 1-, 2+, 2-

  -====2====+
  |    |   /|
  |    |  / |
  |    | /  |
  |    |/   | 
  |    |    |
  |   /|    |
  |  / |    |
  | /  |    |
  |/   |    |
  -====1====+

*/
std::pair<std::vector<float>, std::vector<float>> lineImplAttributes(std::vector<glm::vec2> vertices) {
  std::vector<float> lineVertices = {};
  std::vector<float> lineDirections = {};

  auto pushVertex = [&](glm::vec2 v) {
    lineVertices.push_back(v.x);
    lineVertices.push_back(v.y);
  };

  // 1 1 2 1 2 2
  auto addLine = [&](glm::vec2 v1, glm::vec2 v2) {
    pushVertex(v1);
    pushVertex(v1);
    pushVertex(v2);
    pushVertex(v2);
    pushVertex(v2);
    pushVertex(v1);
  };

  // - + + - + -
  auto addDirections = [&](glm::vec2 v1, glm::vec2 v2) {
    float direction = std::atan2(v2.y - v1.y, v2.x - v1.x);
    float neg = direction + M_PI / 2;
    float pos = direction - M_PI / 2;
    lineDirections.push_back(neg);
    lineDirections.push_back(pos);
    lineDirections.push_back(pos);
    lineDirections.push_back(neg);
    lineDirections.push_back(pos);
    lineDirections.push_back(neg);
  };

  for (size_t i = 0; i < vertices.size(); i++) {
    glm::vec2 v1 = vertices[i];
    glm::vec2 v2 = vertices[(i + 1) % vertices.size()];
    addLine(v1, v2);
    addDirections(v1, v2);
  }

  return { lineVertices, lineDirections };
}

/*
flatten vec2s and polarvec2s
*/
std::vector<float> flattenVertices(std::vector<glm::vec2> vertices) {
  std::vector<float> flatVertices = {};
  for (size_t i = 0; i < vertices.size(); i++) {
    glm::vec2 v = vertices[i];
    flatVertices.push_back(v.x);
    flatVertices.push_back(v.y);
  }
  return flatVertices;
}

std::vector<float> flattenVertices(std::vector<polar_vec2> vertices) {
  std::vector<float> flatVertices = {};
  for (size_t i = 0; i < vertices.size(); i++) {
    glm::vec2 v = vertices[i].xy();
    flatVertices.push_back(v.x);
    flatVertices.push_back(v.y);
  }
  return flatVertices;
}

/*
input array of vec2s, polarvec2s, or numbers
return array of numbers [all vec2]
*/
std::vector<float> getEarcutVertices(std::vector<glm::vec2> vertices) {
  std::vector<float> flatVertices = flattenVertices(vertices);
  std::vector<N> indices = mapbox_earcut(flatVertices);

  std::vector<float> outputVertices = {};
  for (size_t i = 0; i < indices.size(); i++) {
    N i0 = 2 * indices[i];
    outputVertices.push_back(flatVertices[i0]);
    outputVertices.push_back(flatVertices[i0 + 1]);
  }
  return outputVertices;
}


std::vector<N> mapbox_earcut(std::vector<float> vertices) {
  // convert vertices to {{x1, y1}, {x2, y2}, ...} format
  std::vector<std::vector<Point>> polygon;
  std::vector<Point> outer; // we don't have holes

  for (size_t i = 0; i < vertices.size(); i += 2) {
    outer.push_back({vertices[i], vertices[i + 1]});
  }
  polygon.push_back(outer);

  return mapbox::earcut<N>(polygon);
}