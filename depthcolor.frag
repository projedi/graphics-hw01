#version 330 core

uniform float near;
uniform float far;
uniform bool mode;
uniform mat4 P;
in float z;
out vec3 color;

void main() {
   float c1 = clamp((-z - near) / (far - near), 0, 1);
   float zrestored = (2 * gl_FragCoord.z / gl_FragCoord.w - P[3][2]) / (P[2][2] - 1);
   float c2 = clamp((-zrestored - near) / (far - near), 0, 1);
   float c = mode ? c1 : c2;
   color = vec3(c,c,c);
}
