//#include <iostream>
//#include <stdexcept>

//#include <GL/glew.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include "gl_context.h"
#include "camera.h"
#include "model.h"

int main() {
   gl_context context(800, 600);
   context.set_shader("simplevertexshader.vert", "simplefragmentshader.frag");
   GLuint mvp_id = glGetUniformLocation(context.get_shader(), "MVP");
   camera camera(context.main_window());
   model model;

   glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);

   context.main_loop([&]() {
      context.use_shader();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      camera.sendMVP(mvp_id);
      model.draw();
   });

   return 0;
}
