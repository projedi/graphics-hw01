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
   camera camera(context.main_window());
   model model(camera, context);

   glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);

   context.main_loop([&]() {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      camera.update_matrices();
      model.draw();
   });

   return 0;
}
