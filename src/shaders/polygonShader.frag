#version 330 core

in vec4 v_color;
in vec2 v_offset;
out vec4 fragColor;


uniform vec4 u_outline_color;
uniform float u_outline_size;
uniform float u_transition_smoothness;
uniform float u_blend_factor;
uniform float u_draw_outline;

vec4 lerpVec4(vec4 a, vec4 b, float t) {
  return a * (1.0 - t) + b * t;
}

const float DRAW_REGULAR = 0.;
const float DRAW_OUTLINE = 1.;
const float DRAW_OUTLINE_CORNER = 2.;

void main(void) {
  // fragColor = v_color;
  // return;
  
  if (u_draw_outline == DRAW_REGULAR) {
    fragColor = v_color; 
  } else if (u_draw_outline == DRAW_OUTLINE) { 
    fragColor = lerpVec4(v_color, u_outline_color, u_blend_factor);
  } else { // DRAW_OUTLINE_CORNER
    float d = distance(v_offset, gl_FragCoord.xy);
    if (d > u_outline_size) { 
      discard;
    }
    fragColor = lerpVec4(v_color, u_outline_color, u_blend_factor);
  }
  
}