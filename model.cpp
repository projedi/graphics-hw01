#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "model.h"
#include "gl_context.h"
#include "camera.h"

void load_obj_file(std::string const& filename, std::vector<glm::vec3> &vertices,
      std::vector<glm::vec2>& uvs, std::vector<glm::vec3> &normals) {
   std::ifstream in(filename);
   if(!in) {
      std::cerr << "Cannot open obj file " << filename << std::endl;
      throw std::logic_error("Can't open file");
   }
   std::vector<glm::vec3> vbuf;
   std::vector<glm::vec3> vnbuf;
   std::vector<glm::vec2> vtbuf;
   std::string line;
   while(std::getline(in, line)) {
      if(line.substr(0,2) == "v ") {
         std::stringstream s(line.substr(2));
         glm::vec3 v;
         s >> v.x >> v.y >> v.z;
         vbuf.push_back(v);
      } else if(line.substr(0,3) == "vn ") {
         std::stringstream s(line.substr(3));
         glm::vec3 vn;
         s >> vn.x >> vn.y >> vn.z;
         vnbuf.push_back(vn);
      } else if(line.substr(0,3) == "vt ") {
         std::stringstream s(line.substr(3));
         glm::vec2 vt;
         s >> vt.x >> vt.y;
         vtbuf.push_back(vt);
      } else if(line.substr(0,2) == "f ") {
         // Won't parse complicated faces.
         std::stringstream s(line.substr(2));
         int v1,vt1,vn1,v2,vt2,vn2,v3,vt3,vn3;
         char skip;
         s >> v1 >> skip >> vt1 >> skip >> vn1
           >> v2 >> skip >> vt2 >> skip >> vn2
           >> v3 >> skip >> vt3 >> skip >> vn3;
         vertices.push_back(vbuf[v1 - 1]);
         vertices.push_back(vbuf[v2 - 1]);
         vertices.push_back(vbuf[v3 - 1]);
         uvs.push_back(vtbuf[vt1 - 1]);
         uvs.push_back(vtbuf[vt2 - 1]);
         uvs.push_back(vtbuf[vt3 - 1]);
         normals.push_back(vnbuf[vn1 - 1]);
         normals.push_back(vnbuf[vn2 - 1]);
         normals.push_back(vnbuf[vn3 - 1]);
    }
  }
}

model::model(gl_context const& context): _context(context) {
   std::vector<glm::vec3> vertices;
   std::vector<glm::vec2> uvs;
   std::vector<glm::vec3> normals;
   load_obj_file("model.obj", vertices, uvs, normals);
   _vertex_count = vertices.size();
   glGenBuffers(1, &_vertexbuffer);
   glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
   glBufferData(GL_ARRAY_BUFFER, _vertex_count * sizeof(glm::vec3), &vertices[0],
         GL_STATIC_DRAW);

   _depth_shader_id = _context.add_shader("depthcolor.vert", "depthcolor.frag");
   _wire_shader_id = _context.add_shader("wirecolor.vert", "wirecolor.frag");
}

model::~model() {
   glDeleteBuffers(1, &_vertexbuffer);

   _context.remove_shader(_depth_shader_id);
   _context.remove_shader(_wire_shader_id);
}

void model::draw() {
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

   camera const* cmr = camera::instance();
   _context.use_shader(_depth_shader_id);
   cmr->sendMVP(glGetUniformLocation(_depth_shader_id, "MVP"));
   glUniform1f(glGetUniformLocation(_depth_shader_id, "near"), cmr->near());
   glUniform1f(glGetUniformLocation(_depth_shader_id, "far"), cmr->far());
   glUniform1f(glGetUniformLocation(_depth_shader_id, "mode"), cmr->mode());
   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
   glPolygonOffset(1, 1);
   glDrawArrays(GL_TRIANGLES, 0, _vertex_count);

   _context.use_shader(_wire_shader_id);
   camera::instance()->sendMVP(glGetUniformLocation(_wire_shader_id, "MVP"));
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
   glDrawArrays(GL_TRIANGLES, 0, _vertex_count);

   glDisableVertexAttribArray(0);
}
