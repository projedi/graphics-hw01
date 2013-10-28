#include <GL/glew.h>
#include <glm/glm.hpp>

struct GLFWwindow;

struct camera {
   camera(GLFWwindow* window);
   glm::mat4 mvp() const { return _mvp; }
   void sendMVP(GLuint mvp_id) const;
   void update_matrices();
   void mouseButtonCallback(int button, int action, int mods);
   void scrollCallback(double xoffset, double yoffset);
   void keyCallback(int key, int scancode, int action, int mods);
   void cursorPosCallback(double xpos, double ypos);
private:
   void zoomOut();
   void zoomIn();
private:
   GLFWwindow* _window;
   bool _rotation_mode;
   GLfloat _fov;
   GLfloat _width;
   GLfloat _height;
   GLfloat _near;
   GLfloat _far;
   glm::vec3 _position;
   glm::vec3 _up;
   glm::mat4 _projection;
   glm::mat4 _view;
   glm::mat4 _model;
   glm::mat4 _mvp;
   double _mouse_x;
   double _mouse_y;
   double _horizontal_angle;
   double _vertical_angle;
};
