#version 330 core

layout(location = 0) in vec3 pos;
uniform mat4 MVP;
out float z;

void main() {
   gl_Position = MVP * vec4(pos, 1);
   z = gl_Position.z;
}
