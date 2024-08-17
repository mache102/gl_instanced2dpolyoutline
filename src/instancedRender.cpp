#include "instancedRender.h"

InstancedRender::InstancedRender() {
  rendererMap.clear();
  circleRenderer = nullptr;
  resetDepthIndex();
}

void InstancedRender::setCircleRenderer(Shader& shader) {
  setShaderUniforms(shader);
  circleRenderer = new InstancedCircles(shader);
}

void InstancedRender::removeCircleRenderer() {
  delete circleRenderer;
  circleRenderer = nullptr;
}

void InstancedRender::resetDepthIndex() {
  depthIndex = 0;
}

void InstancedRender::defineShaderUniforms(glm::vec2 winres, glm::vec4 outlineColor, float outlineSize, float transitionSmoothness, float blendFactor) {
  this->winres = winres;
  this->outlineColor = outlineColor;
  this->outlineSize = outlineSize;
  this->transitionSmoothness = transitionSmoothness;
  this->blendFactor = blendFactor;
}

void InstancedRender::setShaderUniforms(Shader& shader) {
  shader.use();
  shader.setVec2("u_winres", winres);
  shader.setVec4("u_outline_color", outlineColor);
  shader.setFloat("u_outline_size", outlineSize);
  shader.setFloat("u_transition_smoothness", transitionSmoothness);
  shader.setFloat("u_blend_factor", blendFactor);
}

void InstancedRender::setWinres(glm::vec2 winres) {
  this->winres = winres;
  for (auto& [name, renderer] : rendererMap) {
    renderer->shader.use();
    renderer->shader.setVec2("u_winres", winres);
  }
  if (circleRenderer) {
    circleRenderer->shader.use();
    circleRenderer->shader.setVec2("u_winres", winres);
  }
}

void InstancedRender::setOutlineColor(glm::vec4 outlineColor) {
  this->outlineColor = outlineColor; 
  for (auto& [name, renderer] : rendererMap) {
    renderer->shader.use();
    renderer->shader.setVec4("u_outline_color", outlineColor);
  }
  if (circleRenderer) {
    circleRenderer->shader.use();
    circleRenderer->shader.setVec4("u_outline_color", outlineColor);
  }
}

void InstancedRender::setOutlineSize(float outlineSize) {
  this->outlineSize = outlineSize;
  for (auto& [name, renderer] : rendererMap) {
    renderer->shader.use();
    renderer->shader.setFloat("u_outline_size", outlineSize);
  }
  if (circleRenderer) {
    circleRenderer->shader.use();
    circleRenderer->shader.setFloat("u_outline_size", outlineSize);
  }
}

void InstancedRender::setTransitionSmoothness(float transitionSmoothness) {
  this->transitionSmoothness = transitionSmoothness;
  for (auto& [name, renderer] : rendererMap) {
    renderer->shader.use();
    renderer->shader.setFloat("u_transition_smoothness", transitionSmoothness);
  }
  if (circleRenderer) {
    circleRenderer->shader.use();
    circleRenderer->shader.setFloat("u_transition_smoothness", transitionSmoothness);
  }
}

void InstancedRender::setBlendFactor(float blendFactor) {
  this->blendFactor = blendFactor;
  for (auto& [name, renderer] : rendererMap) {
    renderer->shader.use();
    renderer->shader.setFloat("u_blend_factor", blendFactor);
  }
  if (circleRenderer) {
    circleRenderer->shader.use();
    circleRenderer->shader.setFloat("u_blend_factor", blendFactor);
  }
}

void InstancedRender::addRenderer(std::string name, std::vector<glm::vec2> vertices, Shader& shader) {
  setShaderUniforms(shader);
  rendererMap[name] = new InstancedPolygons(vertices, shader);
}

void InstancedRender::removeRenderer(std::string name) {
  auto it = rendererMap.find(name);
  if (it!=rendererMap.end()) {
    delete it->second;
    rendererMap.erase(it);
  }
}

InstancedPolygons* InstancedRender::getRenderer(std::string name) {
  return rendererMap[name];
}

InstancedCircles* InstancedRender::getCircleRenderer() {
  return circleRenderer;
}

void InstancedRender::clearRenderer(std::string name) {
  rendererMap[name]->clearBuffers();
}

void InstancedRender::clearCircleRenderer() {
  if (circleRenderer) {
    circleRenderer->clearBuffers();
  }
}

void InstancedRender::addInstance(std::string name, float rotation, float size, glm::vec2 offset, glm::vec4 color) {
  rendererMap[name]->addInstance(this->depthIndex, rotation, size, offset, color);
  this->depthIndex--; // we decrease so objects are drawn back to front
}

void InstancedRender::addCircleInstance(float size, glm::vec2 offset, glm::vec4 color) {
  if (circleRenderer) {
    circleRenderer->addInstance(this->depthIndex, size, offset, color);
    this->depthIndex--; // we decrease so objects are drawn back to front
  }
}

void InstancedRender::drawRenderer(std::string name) {
  rendererMap[name]->draw(this->depthIndex);
}

void InstancedRender::drawAll() {
  // draw all opaque objects first
  for (auto& [name, renderer] : rendererMap) {
    renderer->draw(this->depthIndex);
  }
  // then finally the transparent objects (circles - edges are smoothstepped)
  if (circleRenderer) {
    circleRenderer->draw(this->depthIndex);
  }
}

void InstancedRender::clearAll() {
  for (auto& [name, renderer] : rendererMap) {
    renderer->clearBuffers();
  }

  if (circleRenderer) {
    circleRenderer->clearBuffers();
  }
}

void InstancedRender::deleteAll() {}

void InstancedRender::updateBuffers(std::string name) {
  rendererMap[name]->updateSharedBuffers();
}

void InstancedRender::updateSharedBuffers() {
  for (auto& [name, renderer] : rendererMap) {
    renderer->updateSharedBuffers();
  }

  if (circleRenderer) {
    circleRenderer->updateSharedBuffers();
  }
}
