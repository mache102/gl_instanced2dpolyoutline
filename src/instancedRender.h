#ifndef INSTANCED_RENDER_H
#define INSTANCED_RENDER_H

#include <unordered_map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "instancedPolygons.h"
#include "instancedCircles.h"

class InstancedRender {
public:
  std::unordered_map<std::string, InstancedPolygons*> rendererMap;
  InstancedCircles* circleRenderer = nullptr;
  int depthIndex = 0;

  glm::vec2 winres;
  glm::vec4 outlineColor;
  float outlineSize;
  float transitionSmoothness;
  float blendFactor;

  InstancedRender();

  void setCircleRenderer(Shader& shader);
  void removeCircleRenderer();

  void resetDepthIndex();

  void defineShaderUniforms(glm::vec2 winres, glm::vec4 outlineColor, float outlineSize, float transitionSmoothness, float blendFactor);
  void setShaderUniforms(Shader& shader);
  void setWinres(glm::vec2 winres);
  void setOutlineColor(glm::vec4 outlineColor);
  void setOutlineSize(float outlineSize);
  void setTransitionSmoothness(float transitionSmoothness);
  void setBlendFactor(float blendFactor);

  void addRenderer(std::string name, std::vector<glm::vec2> vertices, Shader& shader);
  void removeRenderer(std::string name);
  InstancedPolygons* getRenderer(std::string name);
  InstancedCircles* getCircleRenderer();
  
  void clearRenderer(std::string name);
  void clearCircleRenderer();

  void addInstance(std::string name, float rotation, float size, glm::vec2 offset, glm::vec4 color);
  void addCircleInstance(float size, glm::vec2 offset, glm::vec4 color);

  void drawRenderer(std::string name);
  void drawAll();

  void clearAll();
  void deleteAll();

  void updateBuffers(std::string name);
  void updateSharedBuffers();

};


#endif