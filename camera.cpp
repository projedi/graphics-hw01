#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

// TODO: Make it a singleton
static camera* INSTANCE;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
   INSTANCE->mouseButtonCallback(button, action, mods);
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
   INSTANCE->scrollCallback(xoffset, yoffset);
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
   INSTANCE->keyCallback(key, scancode, action, mods);
}
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
   INSTANCE->cursorPosCallback(xpos, ypos);
}

camera::camera(GLFWwindow* window): _fov(45), _near(0.1),
      _far(100.0), _position(glm::vec3(4,3,-3)), _model(1.0), _rotation_mode(false) {
   INSTANCE = this;
   int w, h;
   glfwGetWindowSize(window, &w, &h);
   _width = w;
   _height = h;
   update_matrices();
   glfwSetMouseButtonCallback(window, ::mouseButtonCallback);
   glfwSetScrollCallback(window, ::scrollCallback);
   glfwSetKeyCallback(window, ::keyCallback);
   glfwSetCursorPosCallback(window, ::cursorPosCallback);
}

void camera::update_matrices() {
   _projection = glm::perspectiveFov(_fov, _width, _height, _near, _far);
   _view = glm::lookAt(_position, glm::vec3(0,0,0), glm::vec3(0,1,0));
   _mvp = _projection * _view * _model;
}

void camera::sendMVP(GLuint mvp_id) {
   update_matrices();
   glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &_mvp[0][0]);
}

void camera::mouseButtonCallback(int button, int action, int mods) {
   if(button == GLFW_MOUSE_BUTTON_LEFT)
      _rotation_mode = (action == GLFW_PRESS);
}

void camera::scrollCallback(double xoffset, double yoffset) {
   if(yoffset < 0) zoomOut();
   else if(yoffset > 0) zoomIn();
}

void camera::keyCallback(int key, int scancode, int action, int mods) {
   if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
      zoomIn();
   else if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
      zoomOut();
}

void camera::cursorPosCallback(double xpos, double ypos) {

}

void camera::zoomIn() {
   _position -= glm::normalize(_position);
}

void camera::zoomOut() {
   _position += glm::normalize(_position);
}
