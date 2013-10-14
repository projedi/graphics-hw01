#include <iostream>
#include <fstream>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gl_context.h"

gl_context::gl_context(int width, int height) {
   init_glfw();
   create_glfw_window(width, height);
   glfwMakeContextCurrent(_main_window);
   init_glew();
   glGenVertexArrays(1, &_vertex_array_id);
   glBindVertexArray(_vertex_array_id);
}

gl_context::~gl_context() {
   glfwDestroyWindow(_main_window);
   glfwTerminate();
   glDeleteProgram(_shader_id);
   glDeleteVertexArrays(1, &_vertex_array_id);
}

void gl_context::set_shader(std::string const& vertex_file_path,
                std::string const& fragment_file_path) {
   GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
   GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

   compile_shader(vertex_file_path, vertex_id);
   compile_shader(fragment_file_path, fragment_id);

   // Link the program
   std::cout << "Linking program" << std::endl;
   _shader_id = glCreateProgram();
   glAttachShader(_shader_id, vertex_id);
   glAttachShader(_shader_id, fragment_id);
   glLinkProgram(_shader_id);

   // Check the program
   GLint result = GL_FALSE;
   int info_log_length;
   glGetProgramiv(_shader_id, GL_LINK_STATUS, &result);
   glGetProgramiv(_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
   if(info_log_length > 0) {
      std::string err_message;
      err_message.resize(info_log_length + 1);
      glGetProgramInfoLog(vertex_id, info_log_length, NULL, &err_message[0]);
      std::cerr << err_message << std::endl;
   }

   glDeleteShader(vertex_id);
   glDeleteShader(fragment_id);
}

void gl_context::init_glfw() {
   glfwSetErrorCallback([](int code, const char* msg) {
         std::cerr << "GLFW Error: " << msg << std::endl;
   });
   if(!glfwInit())
      throw std::runtime_error("Failed to initialize GLFW");
}

void gl_context::create_glfw_window(int width, int height) {
   glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   if(!(_main_window = glfwCreateWindow(width, height, "HW01", NULL, NULL))) {
      glfwTerminate();
      throw std::runtime_error("Failed to open GLFW window");
   }
}

void gl_context::init_glew() {
   glewExperimental = GL_TRUE;
   GLenum err;
   if(err = glewInit())
      throw std::runtime_error("Failed to initialize GLEW: " +
            std::string((char*)glewGetErrorString(err)));
}

int gl_context::compile_shader(std::string const& shader_path, GLuint shader_id) {
   std::string code;
   std::ifstream file(shader_path.c_str(), std::ios::in | std::ios::binary);
   if(!file) {
      std::cerr << "Impossible to open " << shader_path << std::endl;
      return -1;
   }
   file.seekg(0, std::ios::end);
   code.resize(file.tellg());
   file.seekg(0, std::ios::beg);
   file.read(&code[0], code.size());
   std::cout << "Compiling shader: " << shader_path << std::endl;
   char const* source_ptr = code.c_str();
   glShaderSource(shader_id, 1, &source_ptr , NULL);
   glCompileShader(shader_id);
   // Check Vertex Shader
   GLint result = GL_FALSE;
   int info_log_length;
   glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
   glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
   if(info_log_length > 0) {
      std::string err_message;
      err_message.resize(info_log_length + 1);
      glGetShaderInfoLog(shader_id, info_log_length, NULL, &err_message[0]);
      std::cerr << err_message << std::endl;
   }
   return 0;
}

void gl_context::main_loop(std::function<void()> body) {
   do {
      body();

      glfwSwapBuffers(_main_window);
      glfwPollEvents();
   } while(glfwGetKey(_main_window, GLFW_KEY_ESCAPE) != GLFW_PRESS
          && !glfwWindowShouldClose(_main_window));
}

void gl_context::use_shader() {
   glUseProgram(get_shader());
}
