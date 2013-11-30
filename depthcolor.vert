#version 330 core

layout(location = 0) in vec3 pos;
uniform mat4 MV;
uniform mat4 P;
out float z;

void main() {
   vec4 tmp = MV * vec4(pos, 1);
   gl_Position = P * tmp;
   z = tmp.z;
}
