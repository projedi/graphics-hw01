#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

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

camera::camera(GLFWwindow* window): _window(window), _fov(45), _near(0.1),
      _far(100.0), _horizontal_angle(0), _vertical_angle(0),
      _position(glm::vec3(4,0,0)), _model(1.0), _rotation_mode(false), _up(glm::vec3(0,0,1)) {
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
   _view = glm::lookAt(_position, glm::vec3(0,0,0), _up);
   _mvp = _projection * _view * _model;
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
}

std::ostream& operator <<(std::ostream& ost, glm::vec3 const& vec) {
   return ost << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

void camera::cursorPosCallback(double xpos, double ypos) {
   if(!_rotation_mode) return;
   double dx = xpos - _mouse_x;
   double dy = ypos - _mouse_y;
   _horizontal_angle += dx / 180 * 3.14;
   _horizontal_angle = glm::mod(_horizontal_angle, 6.28);
   _vertical_angle += dy / 180 * 3.14;
   _vertical_angle = glm::clamp(_vertical_angle, -1.56, 1.56); // almost pi/2
   // TODO: Issues when angle is at its bounds.
   //std::cerr << std::endl << "ROTATING to "
             //<< _horizontal_angle << " " << _vertical_angle
             //<< std::endl;
   //std::cerr << "position is " << _position << std::endl;;
   glm::vec3 right = glm::vec3(-_position.y, _position.x, 0);
   right = glm::normalize(right);
   //std::cerr << "right is " << right << std::endl;
   _up = glm::cross(right, _position);
   //std::cerr << "up is " << _up << std::endl;
   _position = glm::rotate(_position, -(float)dx, _up);
   _position = glm::rotate(_position, (float)dy, right);
   _mouse_x = xpos;
   _mouse_y = ypos;
}

void camera::zoomIn() {
   _position -= glm::normalize(_position);
}

void camera::zoomOut() {
   _position += glm::normalize(_position);
}
