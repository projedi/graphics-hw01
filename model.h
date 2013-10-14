#include <GL/glew.h>

struct model {
   model();
   ~model();
   void draw();
private:
   GLuint _vertexbuffer;
   GLuint _colorbuffer;
};
