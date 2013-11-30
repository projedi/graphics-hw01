#include <GL/glew.h>
#include <glm/glm.hpp>

struct GLFWwindow;

struct camera {
   static void init(GLFWwindow* window);
   static camera* instance() { return INSTANCE; }
   glm::mat4 mvp() const { return _mvp; }
   GLfloat far() const { return _far; }
   GLfloat near() const { return _near; }
   // A terrible hack to get keyboard events into model
   GLboolean mode() const { return _mode; }
   void sendMVP(GLuint mvp_id) const;
   void sendMV(GLuint mvp_id) const;
   void sendP(GLuint mvp_id) const;
   void update_matrices();
   void mouseButtonCallback(int button, int action, int mods);
   void scrollCallback(double xoffset, double yoffset);
   void keyCallback(int key, int scancode, int action, int mods);
   void cursorPosCallback(double xpos, double ypos);
private:
   void zoomOut();
   void zoomIn();
private:
   camera(GLFWwindow* window);
   ~camera();
   static camera* INSTANCE;
   GLFWwindow* _window;
   bool _rotation_mode;
   GLfloat _fov;
   GLfloat _width;
   GLfloat _height;
   GLfloat _near;
   GLfloat _far;
   glm::vec3 _direction;
   glm::vec3 _right;
   glm::mat4 _mvp;
   glm::mat4 _mv;
   glm::mat4 _p;
   double _mouse_x;
   double _mouse_y;
   float _distance;
   GLboolean _mode;
};
