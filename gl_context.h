#pragma once

#include <string>
#include <functional>

#include <GL/glew.h>

struct GLFWwindow;

struct gl_context {
   gl_context(int width, int height);
   ~gl_context();
   void main_loop(std::function<void()> body);
   void set_shader(std::string const& vertex_file_path,
                   std::string const& fragment_file_path);
   GLFWwindow* main_window() const { return _main_window; }
   GLuint get_shader() { return _shader_id; }
   void use_shader();
private:
   void init_glfw();
   void create_glfw_window(int width, int height);
   void init_glew();
   int compile_shader(std::string const& shader_path, GLuint shader_id);
private:
   GLuint _vertex_array_id;
   GLFWwindow* _main_window;
   GLuint _shader_id;
};
