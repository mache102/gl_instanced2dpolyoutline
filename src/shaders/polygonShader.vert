#version 330 core

layout (location = 0) in vec2 a_coord;
layout (location = 1) in float a_outline_direction;

layout (location = 2) in float a_index;
layout (location = 3) in float a_rotation;
layout (location = 4) in float a_size;
layout (location = 5) in vec2 a_offset;
layout (location = 6) in vec4 a_color;

out vec4 v_color;
out vec2 v_offset;

uniform vec2 u_winres;
uniform float u_polygon_count;
uniform float u_outline_size;
uniform float u_draw_outline;

const float DRAW_REGULAR = 0.;
const float DRAW_OUTLINE = 1.;
const float DRAW_OUTLINE_CORNER = 2.;
const float SQRT2 = 1.41421356237;

vec2 rotateVec2(vec2 v, float angle) {
  return vec2(
    v.x * cos(angle) - v.y * sin(angle),
    v.x * sin(angle) + v.y * cos(angle)
  );
}

void main(void) {
  v_color = a_color;
  vec2 half_winres = u_winres / 2.0;
  v_offset = vec2(0.);

  vec2 coord;
  float z = a_index;
  if (u_draw_outline == DRAW_REGULAR) {

    coord = rotateVec2(a_coord, a_rotation) * a_size + a_offset;
    z /= u_polygon_count;  

  } else if (u_draw_outline == DRAW_OUTLINE) {
    coord = rotateVec2(a_coord, a_rotation) * a_size + a_offset;
    coord += vec2(
      cos(a_outline_direction + a_rotation), 
      sin(a_outline_direction + a_rotation)
    ) * u_outline_size;
    z = (z - 0.5) / u_polygon_count;

  } else { // DRAW_OUTLINE_CORNER
    coord = rotateVec2(a_coord, a_rotation) * a_size + a_offset;
    v_offset = coord + half_winres;

    coord += vec2(
      cos(a_outline_direction), 
      sin(a_outline_direction)
    ) * u_outline_size * SQRT2;
    z = (z - 0.5) / float(u_polygon_count); 

    
  }

  gl_Position = vec4(coord / half_winres, z, 1.0);
}