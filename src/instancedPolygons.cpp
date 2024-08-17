#include "instancedPolygons.h"

InstancedPolygons::InstancedPolygons(std::vector<glm::vec2> vertices, Shader shader) {
  this->shader = shader;

  // cached attr locations
  this->coordLoc = 0;
  this->outlineDirectionLoc = 1;

  this->indexLoc = 2;
  this->rotationLoc = 3;
  this->sizeLoc = 4;
  this->offsetLoc = 5;
  this->colorLoc = 6;
  
  this->clearBuffers();
  this->genBuffers();
  this->bindVertexAttributes();

  this->setCoords(getEarcutVertices(vertices));
  this->setOutlineCoords(lineImplAttributes(vertices));
  this->setCornerCoords(outlineRoundedCorners(vertices));
}

void InstancedPolygons::setCoords(std::vector<float> vertices) {
  this->coords = vertices;
  updatePolygonBuffer();
}

void InstancedPolygons::setOutlineCoords(std::pair<std::vector<float>, std::vector<float>> lineVerticesDirections) {
  setOutlineCoords(lineVerticesDirections.first, lineVerticesDirections.second);
}

void InstancedPolygons::setOutlineCoords(std::vector<float> lineVertices, std::vector<float> lineDirections) {
  this->outlineCoords = lineVertices;
  this->outlineDirections = lineDirections;
  updateOutlineBuffers();
}

void InstancedPolygons::setCornerCoords(std::pair<std::vector<float>, std::vector<float>> cornerVerticesDirections) {
  setCornerCoords(cornerVerticesDirections.first, cornerVerticesDirections.second);
}

void InstancedPolygons::setCornerCoords(std::vector<float> cornerVertices, std::vector<float> cornerDirections) {
  this->cornerCoords = cornerVertices;
  this->cornerDirections = cornerDirections;
  updateCornerBuffers();
}

void InstancedPolygons::genBuffers() {
  glGenVertexArrays(1, &this->vao);

  glGenBuffers(1, &this->coordBuffer);
  glGenBuffers(1, &this->outlineDirectionBuffer);

  // shared
  glGenBuffers(1, &this->indexBuffer);
  glGenBuffers(1, &this->rotationBuffer);
  glGenBuffers(1, &this->sizeBuffer);
  glGenBuffers(1, &this->offsetBuffer);
  glGenBuffers(1, &this->colorBuffer);  
}

void InstancedPolygons::bindVertexAttributes() {
  this->shader.use();

  glBindVertexArray(this->vao);

  glBindBuffer(GL_ARRAY_BUFFER, this->coordBuffer);
  glVertexAttribPointer(this->coordLoc, 2, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(this->coordLoc);

  glBindBuffer(GL_ARRAY_BUFFER, this->outlineDirectionBuffer);
  glVertexAttribPointer(this->outlineDirectionLoc, 1, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(this->outlineDirectionLoc);



  glBindBuffer(GL_ARRAY_BUFFER, this->indexBuffer);
  glVertexAttribPointer(this->indexLoc, 1, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(this->indexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, this->rotationBuffer);
  glVertexAttribPointer(this->rotationLoc, 1, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(this->rotationLoc);

  glBindBuffer(GL_ARRAY_BUFFER, this->sizeBuffer);
  glVertexAttribPointer(this->sizeLoc, 1, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(this->sizeLoc);

  glBindBuffer(GL_ARRAY_BUFFER, this->offsetBuffer);
  glVertexAttribPointer(this->offsetLoc, 2, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(this->offsetLoc);

  glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
  glVertexAttribPointer(this->colorLoc, 4, GL_UNSIGNED_BYTE, true, 0, 0);
  glEnableVertexAttribArray(this->colorLoc);

  glVertexAttribDivisor(this->indexLoc, 1);
  glVertexAttribDivisor(this->rotationLoc, 1);
  glVertexAttribDivisor(this->sizeLoc, 1);
  glVertexAttribDivisor(this->offsetLoc, 1);
  glVertexAttribDivisor(this->colorLoc, 1);
}


void InstancedPolygons::clearBuffers() {
  this->indexs = {};
  this->rotations = {};
  this->sizes = {};
  this->offsets = {};
  this->colors = {};
} 

void InstancedPolygons::addInstance(int index, float rotation, float size, glm::vec2 offset, glm::vec4 color) {
  this->indexs.push_back(index);
  this->rotations.push_back(rotation);
  this->sizes.push_back(size);
  this->offsets.push_back(offset);

  this->colors.push_back(color.r);
  this->colors.push_back(color.g);
  this->colors.push_back(color.b);
  this->colors.push_back(color.a);
}


void InstancedPolygons::updatePolygonBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, this->coordBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->coords.size() * sizeof(float), this->coords.data(), GL_STATIC_DRAW);
}

void InstancedPolygons::updateOutlineBuffers() {
  glBindBuffer(GL_ARRAY_BUFFER, this->coordBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->outlineCoords.size() * sizeof(float), this->outlineCoords.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, this->outlineDirectionBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->outlineDirections.size() * sizeof(float), this->outlineDirections.data(), GL_STATIC_DRAW);
}

void InstancedPolygons::updateCornerBuffers() {
  glBindBuffer(GL_ARRAY_BUFFER, this->coordBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->cornerCoords.size() * sizeof(float), this->cornerCoords.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, this->outlineDirectionBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->cornerDirections.size() * sizeof(float), this->cornerDirections.data(), GL_STATIC_DRAW);
}

void InstancedPolygons::updateIndexBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, this->indexBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->indexs.size() * sizeof(float), this->indexs.data(), GL_STATIC_DRAW);
}

void InstancedPolygons::updateRotationBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, this->rotationBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->rotations.size() * sizeof(float), this->rotations.data(), GL_STATIC_DRAW);
}

void InstancedPolygons::updateSizeBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, this->sizeBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->sizes.size() * sizeof(float), this->sizes.data(), GL_STATIC_DRAW);
}

void InstancedPolygons::updateOffsetBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, this->offsetBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->offsets.size() * sizeof(glm::vec2), this->offsets.data(), GL_STATIC_DRAW);
}

void InstancedPolygons::updateColorBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(uint8_t), this->colors.data(), GL_STATIC_DRAW);
}

void InstancedPolygons::updateSharedBuffers() {
  updateIndexBuffer();
  updateRotationBuffer();
  updateSizeBuffer();
  updateOffsetBuffer();
  updateColorBuffer();
}

void InstancedPolygons::draw(int maxDepthIndex) {
  size_t instanceCount = this->indexs.size();
  if (instanceCount == 0) {
    return;
  }

  shader.use();
  shader.setFloat("u_polygon_count", maxDepthIndex);

  glBindVertexArray(this->vao);

  // 1. main polygon
  shader.setFloat("u_draw_outline", 0.0);
  updatePolygonBuffer();
  glDrawArraysInstanced(GL_TRIANGLES, 0, this->coords.size() / 2, instanceCount);
  
  // 2. outline
  shader.setFloat("u_draw_outline", 1.0);
  updateOutlineBuffers();
  glDrawArraysInstanced(GL_TRIANGLES, 0, this->outlineCoords.size() / 2, instanceCount);

  // 3. corners
  shader.setFloat("u_draw_outline", 2.0);
  updateCornerBuffers();
  glDrawArraysInstanced(GL_TRIANGLES, 0, this->cornerCoords.size() / 2, instanceCount);
}