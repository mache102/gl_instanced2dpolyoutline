#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "rgba_color.h"
#include "timer.h"
#include "settings.h"
#include "utils.h"
#include "instancedRender.h"

constexpr int OPENGL_VERSION_MAJOR = 3;
constexpr int OPENGL_VERSION_MINOR = 3;

using namespace settings;

GLFWwindow* init_glfw();
bool init_opengl();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void error_callback(int error, const char* description);

Shader* p_shader = nullptr; 

int64_t tick = 0;
InstancedRender renderManager;

void update() {
  for (auto& [name, renderer] : renderManager.rendererMap) {
    std::vector<float>& rotations = renderer->rotations;
    for (uint32_t i = 0; i < rotations.size(); i++) {
      rotations[i] += 0.1;
      if (rotations[i] > 2 * M_PI) {
        rotations[i] -= 2 * M_PI;
      }
    }
    renderer->updateRotationBuffer();
  }
}

float radians(float degrees) {
  return degrees * M_PI / 180.0;
}

int main(int argc, char** argv) {

  // fixed seed
  srand(seed);
  GLFWwindow* window = init_glfw();
  if (window == nullptr) {
    return -1;
  }
  if (!init_opengl()) {
    return -1;
  }
  parse_config("./config.txt");

  renderManager = InstancedRender();
  renderManager.defineShaderUniforms(winres, outlineColor.rgba(true), outlineSize, transitionSmoothness, blendFactor);

  std::vector<glm::vec2> starVertices = {
    polar_vec2(3,radians(0)).xy(),
    polar_vec2(1.50, radians(45)).xy(),
    polar_vec2(3,radians(90)).xy(),
    polar_vec2(1.50, radians(135)).xy(),
    polar_vec2(3,radians(180)).xy(),
    polar_vec2(1.50, radians(225)).xy(),
    polar_vec2(3,radians(270)).xy(),
    polar_vec2(1.50, radians(315)).xy()
  };
  Shader polygonShader = Shader("./src/shaders/polygonShader.vert", "./src/shaders/polygonShader.frag");
  renderManager.addRenderer("star", starVertices, polygonShader);

  std::vector<glm::vec2> squareVertices = {
    glm::vec2(-1, -1),
    glm::vec2(1, -1),
    glm::vec2(1, 1),
    glm::vec2(-1, 1)
  };
  Shader squareShader = Shader("./src/shaders/polygonShader.vert", "./src/shaders/polygonShader.frag");
  renderManager.addRenderer("square", squareVertices, squareShader);

  std::vector<glm::vec2> triangleVertices = {
    glm::vec2(-1, 0),
    glm::vec2(1, 0),
    glm::vec2(0, 4)
  };
  Shader triangleShader = Shader("./src/shaders/polygonShader.vert", "./src/shaders/polygonShader.frag");
  renderManager.addRenderer("triangle", triangleVertices, triangleShader);

  Timer timer_init = Timer("init", true);
  for (uint32_t i = 0; i < polygonCount; i++) {
    renderManager.addInstance(
      "star", 
      i * 2 * M_PI / 360.0,
      minSize + (maxSize - minSize) * (rand() / (float)RAND_MAX), 
      randCoord(),
      polygonColors[i % polygonColors.size()].rgba()
    );

    renderManager.addInstance(
      "square", 
      i * 2 * M_PI / 360.0,
      minSize + (maxSize - minSize) * (rand() / (float)RAND_MAX), 
      randCoord(),
      polygonColors[i % polygonColors.size()].rgba()
    );

    renderManager.addInstance(
      "triangle", 
      i * 2 * M_PI / 360.0,
      minSize + (maxSize - minSize) * (rand() / (float)RAND_MAX), 
      randCoord(),
      polygonColors[i % polygonColors.size()].rgba()
    );
  }

  renderManager.updateSharedBuffers();
  timer_init.end();

  Timer render_timer = Timer("render", false);
  Timer update_timer = Timer("update", false);  

  double curr_time, last_time, delta_time;
  delta_time = 0.; // get rid of an unused var warning

  std::cout << "bg color: " << (int)bgColor.r << " " << (int)bgColor.g << " " << (int)bgColor.b << " " << (int)bgColor.a << std::endl;
  last_time = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    curr_time = glfwGetTime();
    delta_time = curr_time - last_time;
    last_time = curr_time;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update_timer.start();
    if (tick_updates > 0) {
      update();
    }
    update_timer.end(true, false);

    renderManager.drawAll();

    render_timer.start();
    glfwSwapBuffers(window);
    render_timer.end(true, false);
    glfwPollEvents();

    tick++;

    if (tick % print_every == 0) {
      std::cout << "tick = " << tick << std::endl;
      update_timer.print_report();
      update_timer.reset_durations();
      render_timer.print_report();
      render_timer.reset_durations();

    }

    if (max_fps > 0) {
      float desired_frame_time = 1.0 / max_fps;
      if (delta_time < desired_frame_time)
      {
        float delay_time = desired_frame_time - delta_time;
        std::chrono::milliseconds delay_ms((int)(delay_time * 1000));
        std::this_thread::sleep_for(delay_ms);
      }
    }
  }

  glfwTerminate();

  return 0;
}

GLFWwindow* init_glfw() {
  std::cout << "Initializing GLFW\n";
  if(!glfwInit()) {
    std::cout << "Failed to initialize GLFW\n";
    return nullptr;
  }

  glfwSetErrorCallback(error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);

  std::cout << "Creating window\n";
  GLFWwindow* window = glfwCreateWindow(winres.x, winres.y, window_title.c_str(), NULL, NULL);

  if (!window) {
    std::cout << "Failed to create GLFW window\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);
  
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return window;
}

bool init_opengl() {
  std::cout << "Initializing GLAD\n";
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return false;
  }
  std::cout << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << " loaded\n";

  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);

  // no vsync
  glfwSwapInterval(vsync);

  const GLubyte* renderer = glGetString(GL_RENDERER); // Get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // Version as a string
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "OpenGL version supported " << version << std::endl;

  return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);

  winres.x = width;
  winres.y = height;
  aspect_ratio = (float)width / (float)height;

  renderManager.setWinres(winres);
}


void error_callback(int error, const char* description) {
    std::cerr << "GLFW error " << error << ": " << description << "\n";
}