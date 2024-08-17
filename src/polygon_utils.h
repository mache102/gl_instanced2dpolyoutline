#ifndef POLYGON_UTILS_H
#define POLYGON_UTILS_H

#include <vector>
#include <array>
#include <cmath>
#include <iostream>

#include "mapbox/earcut.hpp"

#include "polar_vec2.h"

using Coord = float;
using N = uint16_t;
using Point = std::array<Coord, 2>;

extern const std::vector<float> roundedCornerDirections;

std::pair<std::vector<float>, std::vector<float>> outlineRoundedCorners(std::vector<glm::vec2> vertices);
std::pair<std::vector<float>, std::vector<float>> lineImplAttributes(std::vector<glm::vec2> vertices);

std::vector<float> flattenVertices(std::vector<glm::vec2> vertices);
std::vector<float> flattenVertices(std::vector<polar_vec2> vertices);

std::vector<float> getEarcutVertices(std::vector<glm::vec2> vertices);

std::vector<N> mapbox_earcut(std::vector<float> vertices);

#endif