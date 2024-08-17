#ifndef POLAR_VEC2_H
#define POLAR_VEC2_H

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream> // For std::ostringstream
#include <iomanip> // For std::setprecision and std::fixed


struct polar_vec2 {
  float r;
  float theta;

  enum input_type {
    Polar,
    Cartesian
  };

  polar_vec2();
  polar_vec2(float a, float b, input_type i_type = Polar);
  polar_vec2(glm::vec2 vec, input_type i_type = Polar);

  // returns cartesian coordinates
  glm::vec2 xy();

  polar_vec2& scale(float factor);
  polar_vec2 scaled(float factor) const;
  polar_vec2& rotate(float angle);
  polar_vec2 rotated(float angle) const;

  std::string print(int precision = 4);

  static std::vector<polar_vec2> create_polar_vec2s(std::vector<glm::vec2> vecs, polar_vec2::input_type i_type = polar_vec2::Cartesian);

  static std::vector<glm::vec2> polar_vec2s_to_cartesian(std::vector<polar_vec2> polar);
};

std::vector<glm::vec2> glm_vec2s_to_cartesian(std::vector<glm::vec2> vecs);
std::vector<glm::vec2> rotate_polar_vertices(std::vector<polar_vec2> vertices, float rotation);

glm::vec2 polar_to_xy(glm::vec2 polar);
glm::vec2 xy_to_polar(glm::vec2 xy);


#endif