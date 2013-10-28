#include <GL/glew.h>

struct gl_context;
struct camera;

struct model {
   model(gl_context const& context);
   ~model();
   void draw();
private:
   GLuint _vertexbuffer;
   GLuint _depth_shader_id;
   GLuint _wire_shader_id;
   gl_context const& _context;
   GLsizei _vertex_count;
};
