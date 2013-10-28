#include <GL/glew.h>

struct gl_context;
struct camera;

struct model {
   model(camera const& camera, gl_context const& context);
   ~model();
   void draw();
private:
   GLuint _vertexbuffer;
   GLuint _colorbuffer;
   GLuint _depth_shader_id;
   GLuint _wire_shader_id;
   gl_context const& _context;
   camera const& _camera;
};
