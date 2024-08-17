#include "polar_vec2.h"

polar_vec2::polar_vec2() : r(0), theta(0) {}
polar_vec2::polar_vec2(float a, float b, input_type i_type) {
  if (i_type == Polar) {
    this->r = a;
    this->theta = b;
  } else {
    this->r = sqrt(a * a + b * b);
    // a=x, b=y
    this->theta = atan2(b, a);
  }
}
polar_vec2::polar_vec2(glm::vec2 vec, input_type i_type) {
  if (i_type == Polar) {
    this->r = vec.x;
    this->theta = vec.y;
  } else {
    this->r = sqrt(vec.x * vec.x + vec.y * vec.y);
    this->theta = atan2(vec.y, vec.x);
  }
}

// returns cartesian coordinates
glm::vec2 polar_vec2::xy() {
  return glm::vec2(r * cos(theta), r * sin(theta));
}

std::string polar_vec2::print(int precision) {
  std::ostringstream out;

  // Use fixed floating-point notation and set precision
  out << std::fixed << std::setprecision(precision);
  out << "polar_vec2(" << r << ", " << theta << ")";

  return out.str();
}

polar_vec2& polar_vec2::scale(float factor) {
  this->r *= factor;
  return *this;
}

polar_vec2 polar_vec2::scaled(float factor) const {
  polar_vec2 result = *this;
  result.r *= factor; 
  return result;
}

polar_vec2& polar_vec2::rotate(float angle) {
  this->theta += angle;
  return *this;
}

polar_vec2 polar_vec2::rotated(float angle) const {
  polar_vec2 result = *this; 
  result.theta += angle; 
  return result;
}

std::vector<polar_vec2> polar_vec2::create_polar_vec2s(std::vector<glm::vec2> vecs, polar_vec2::input_type i_type) {
  std::vector<polar_vec2> polar;
  for (glm::vec2 vec : vecs) {
    polar.push_back(polar_vec2(vec, i_type));
  }
  return polar;
}

std::vector<glm::vec2> polar_vec2::polar_vec2s_to_cartesian(std::vector<polar_vec2> polar) {
  std::vector<glm::vec2> cartesian;
  for (polar_vec2 vec : polar) {
    cartesian.push_back(vec.xy());
  }
  return cartesian;
}

// starts with polar.
std::vector<glm::vec2> glm_vec2s_to_cartesian(std::vector<glm::vec2> vecs) {
  std::vector<glm::vec2> cartesian;
  for (glm::vec2 vec : vecs) {
    cartesian.push_back(glm::vec2(vec.x * cos(vec.y), vec.x * sin(vec.y)));
  }
  return cartesian;

}

std::vector<glm::vec2> rotate_polar_vertices(std::vector<polar_vec2> vertices, float rotation) {
  std::vector<glm::vec2> ret;
  for (auto v : vertices) {
    ret.push_back(v.rotated(rotation).xy());
  }
  return ret;
}

glm::vec2 polar_to_xy(glm::vec2 polar) {
  return glm::vec2(polar.x * cos(polar.y), polar.x * sin(polar.y));
}
glm::vec2 xy_to_polar(glm::vec2 xy) {
  return glm::vec2(sqrt(xy.x * xy.x + xy.y * xy.y), atan2(xy.y, xy.x));
}