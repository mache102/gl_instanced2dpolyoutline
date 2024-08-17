#ifndef INSTANCED_CIRCLES_H
#define INSTANCED_CIRCLES_H

#include <unordered_map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"

class InstancedCircles {
public:
  std::vector<float> coords;

  std::vector<float> indexs;
  std::vector<float> sizes;
  std::vector<glm::vec2> offsets;
  std::vector<glm::vec4> colors;

  Shader shader;
  GLuint vao;
  GLuint coordBuffer, indexBuffer, sizeBuffer, offsetBuffer, colorBuffer;
  GLuint coordLoc,    indexLoc, sizeLoc, offsetLoc, colorLoc;

  InstancedCircles(Shader shader);
  void genBuffers();
  void bindVertexAttributes();
  void clearBuffers();
  void addInstance(int index, float size, glm::vec2 offset, glm::vec4 color);

  void updateCoordBuffer();

  void updateIndexBuffer();
  void updateSizeBuffer();
  void updateOffsetBuffer();
  void updateColorBuffer();

  void updateSharedBuffers();

  void draw(int maxDepthIndex);

};

#endif