#include <GL/glew.h>
#include <glm/glm.hpp>

struct GLFWwindow;

struct camera {
   camera(GLFWwindow* window);
   glm::mat4 mvp() { return _mvp; }
   void sendMVP(GLuint mvp_id);
public:
   void mouseButtonCallback(int button, int action, int mods);
   void scrollCallback(double xoffset, double yoffset);
   void keyCallback(int key, int scancode, int action, int mods);
   void cursorPosCallback(double xpos, double ypos);
private:
   void update_matrices();
   void zoomOut();
   void zoomIn();
private:
   bool _rotation_mode;
   GLfloat _fov;
   GLfloat _width;
   GLfloat _height;
   GLfloat _near;
   GLfloat _far;
   glm::vec3 _position;
   glm::mat4 _projection;
   glm::mat4 _view;
   glm::mat4 _model;
   glm::mat4 _mvp;
};
