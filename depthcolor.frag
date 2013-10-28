#version 330 core

in float z;
out vec3 color;

void main() {
   float z1 = 2 * gl_FragCoord.z - 1;
   // I don't quite get why results are different
   /*float c = (-z + 1) / 2;*/
   float c = (-z1 + 1) / 2; // which = -gl_FragCoord.z
   color = vec3(c,c,c);
}
