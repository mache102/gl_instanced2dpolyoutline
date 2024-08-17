#include "instancedCircles.h"

InstancedCircles::InstancedCircles(Shader shader) {
  this->shader = shader;  

  this->clearBuffers();
  this->genBuffers();
  this->bindVertexAttributes();

  this->coords = {
    -1, -1, 
    -1, 1,
    1, 1,
    -1, -1, 
    1, 1,
    1, -1
  };

  this->coordLoc = 0;

  this->indexLoc = 1;
  this->sizeLoc = 2;
  this->offsetLoc = 3;
  this->colorLoc = 4;
  
  updateCoordBuffer();
}

void InstancedCircles::genBuffers() {
  glGenVertexArrays(1, &this->vao);

  glGenBuffers(1, &this->indexBuffer);
  glGenBuffers(1, &this->coordBuffer);
  glGenBuffers(1, &this->sizeBuffer);
  glGenBuffers(1, &this->offsetBuffer);
  glGenBuffers(1, &this->colorBuffer);
}

void InstancedCircles::bindVertexAttributes() {
  this->shader.use();


  glBindVertexArray(this->vao);

  glBindBuffer(GL_ARRAY_BUFFER, this->indexBuffer);
  glVertexAttribPointer(indexLoc, 1, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(indexLoc);
  
  glBindBuffer(GL_ARRAY_BUFFER, this->coordBuffer);
  glVertexAttribPointer(coordLoc, 2, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(coordLoc);

  glBindBuffer(GL_ARRAY_BUFFER, this->sizeBuffer);
  glVertexAttribPointer(sizeLoc, 1, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(sizeLoc);
  
  glBindBuffer(GL_ARRAY_BUFFER, this->offsetBuffer);
  glVertexAttribPointer(offsetLoc, 2, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(offsetLoc);
  
  glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
  glVertexAttribPointer(colorLoc, 4, GL_UNSIGNED_BYTE, true, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glVertexAttribDivisor(indexLoc, 1);
  glVertexAttribDivisor(sizeLoc, 1);
  glVertexAttribDivisor(offsetLoc, 1);
  glVertexAttribDivisor(colorLoc, 1);
}
 
void InstancedCircles::clearBuffers() {
  this->indexs = {};
  this->sizes = {};
  this->offsets = {};
  this->colors = {};
}

void InstancedCircles::addInstance(int index, float size, glm::vec2 offset, glm::vec4 color) {
  this->indexs.push_back((float)index);
  this->sizes.push_back(size);
  this->offsets.push_back(offset);
  this->colors.push_back(color);
}

void InstancedCircles::updateCoordBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, this->coordBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->coords.size() * sizeof(float), this->coords.data(), GL_STATIC_DRAW);
}

void InstancedCircles::updateIndexBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, this->indexBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->indexs.size() * sizeof(float), this->indexs.data(), GL_STATIC_DRAW);
}

void InstancedCircles::updateSizeBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, this->sizeBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->sizes.size() * sizeof(float), this->sizes.data(), GL_STATIC_DRAW);
}

void InstancedCircles::updateOffsetBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, this->offsetBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->offsets.size() * sizeof(glm::vec2), this->offsets.data(), GL_STATIC_DRAW);
}

void InstancedCircles::updateColorBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, this->colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(glm::vec4), this->colors.data(), GL_STATIC_DRAW);
}

void InstancedCircles::updateSharedBuffers() {
  updateIndexBuffer();
  updateSizeBuffer();
  updateOffsetBuffer();
  updateColorBuffer();
}

void InstancedCircles::draw(int maxDepthIndex) {
  shader.use();
  shader.setFloat("u_polygon_count", (float)abs(maxDepthIndex));
  glDrawArraysInstanced(GL_TRIANGLES, 0, coords.size() / 2, indexs.size());
}