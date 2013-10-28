#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "camera.h"

camera* camera::INSTANCE;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
   camera::instance()->mouseButtonCallback(button, action, mods);
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
   camera::instance()->scrollCallback(xoffset, yoffset);
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
   camera::instance()->keyCallback(key, scancode, action, mods);
}
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
   camera::instance()->cursorPosCallback(xpos, ypos);
}

void camera::init(GLFWwindow* window) {
   if(INSTANCE) return;
   INSTANCE = new camera(window);
}

camera::camera(GLFWwindow* window): _window(window), _fov(45), _near(1.0),
      _far(30.0), _direction(1,0,0), _right(0,1,0), _distance(10),
      _rotation_mode(false), _mode(false) {
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
   glm::vec3 position = _distance * _direction;
   glm::vec3 up = glm::cross(_right, _direction);
   glm::mat4 projection = glm::perspectiveFov(_fov, _width, _height, _near, _far);
   glm::mat4 view = glm::lookAt(position, glm::vec3(0,0,0), up);
   glm::mat4 model(1.0);
   _mvp = projection * view * model;
}

void camera::sendMVP(GLuint mvp_id) const {
   glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &_mvp[0][0]);
}

void camera::mouseButtonCallback(int button, int action, int mods) {
   if(button == GLFW_MOUSE_BUTTON_LEFT) {
      _rotation_mode = (action == GLFW_PRESS);
      if(_rotation_mode) {
         glfwGetCursorPos(_window, &_mouse_x, &_mouse_y);
      }
   }
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
   else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
      _mode = !_mode;
}

std::ostream& operator <<(std::ostream& ost, glm::vec3 const& vec) {
   return ost << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

void camera::cursorPosCallback(double xpos, double ypos) {
   if(!_rotation_mode) return;
   double dx = xpos - _mouse_x;
   double dy = ypos - _mouse_y;
   float up_angle = -dx;
   float right_angle = dy;
   glm::vec3 up = glm::cross(_right, _direction);
   _direction = glm::rotate(glm::rotate(_direction, up_angle, up), right_angle, _right);
   _right = glm::rotate(glm::rotate(_right, up_angle, up), right_angle, _right);
   _mouse_x = xpos;
   _mouse_y = ypos;
}

void camera::zoomIn() {
   _distance = glm::max(_distance - 1, 0.1f);
}

void camera::zoomOut() {
   _distance = glm::min(_distance + 1, 1000.0f);
}
