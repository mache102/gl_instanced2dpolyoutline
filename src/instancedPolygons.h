#ifndef INSTANCED_POLYGONS_H
#define INSTANCED_POLYGONS_H

#include <unordered_map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "polygon_utils.h"

class InstancedPolygons {
public:
  std::vector<float> coords;
  std::vector<float> outlineCoords, outlineDirections;
  std::vector<float> cornerCoords, cornerDirections;

  std::vector<float> indexs;
  std::vector<float> rotations;
  std::vector<float> sizes;
  std::vector<glm::vec2> offsets;
  std::vector<uint8_t> colors;

  Shader shader;
  GLuint vao;
  GLuint coordBuffer, outlineDirectionBuffer, indexBuffer, rotationBuffer, sizeBuffer, offsetBuffer, colorBuffer;
  GLuint coordLoc, outlineDirectionLoc,       indexLoc, rotationLoc, sizeLoc, offsetLoc, colorLoc;

  InstancedPolygons(std::vector<glm::vec2> vertices, Shader shader);

  void setCoords(std::vector<float> vertices);

  void setOutlineCoords(std::pair<std::vector<float>, std::vector<float>> lineVerticesDirections);
  void setOutlineCoords(std::vector<float> lineVertices, std::vector<float> lineDirections);
  void setCornerCoords(std::pair<std::vector<float>, std::vector<float>> cornerVerticesDirections);
  void setCornerCoords(std::vector<float> cornerVertices, std::vector<float> cornerDirections);

  void genBuffers();

  void bindVertexAttributes();  

  void clearBuffers();
  void addInstance(int index, float rotation, float size, glm::vec2 offset, glm::vec4 color);

  void updatePolygonBuffer();
  void updateOutlineBuffers();
  void updateCornerBuffers();

  void updateIndexBuffer();
  void updateRotationBuffer();
  void updateSizeBuffer();
  void updateOffsetBuffer();
  void updateColorBuffer();

  void updateSharedBuffers();

  void draw(int maxDepthIndex);

};

#endif