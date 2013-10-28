#version 330 core

uniform float near;
uniform float far;
uniform bool mode;
in float z;
out vec3 color;

void main() {
   float c1 = clamp((z - near) / (far - near), 0, 1);
   float c2 = clamp(((2 * gl_FragCoord.z - 1) - gl_DepthRange.near)
      / (gl_DepthRange.far - gl_DepthRange.near), 0, 1);
   float c = mode ? c1 : c2;
   color = vec3(c,c,c);
}
