#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int compile_shader(std::string const& shader_path, GLuint shader_id) {
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

GLuint load_shaders(std::string const& vertex_file_path,
                    std::string const& fragment_file_path) {

   GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
   GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

   if(compile_shader(vertex_file_path, vertex_id)) return 0;
   compile_shader(fragment_file_path, fragment_id);

   // Link the program
   std::cout << "Linking program" << std::endl;
   GLuint program_id = glCreateProgram();
   glAttachShader(program_id, vertex_id);
   glAttachShader(program_id, fragment_id);
   glLinkProgram(program_id);

   // Check the program
   GLint result = GL_FALSE;
   int info_log_length;
   glGetProgramiv(program_id, GL_LINK_STATUS, &result);
   glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
   if(info_log_length > 0) {
      std::string err_message;
      err_message.resize(info_log_length + 1);
      glGetProgramInfoLog(vertex_id, info_log_length, NULL, &err_message[0]);
      std::cerr << err_message << std::endl;
   }

   glDeleteShader(vertex_id);
   glDeleteShader(fragment_id);

   return program_id;
}

glm::vec3 position = glm::vec3(0, 0, 5);
float horizontal_angle = 3.14;
float vertical_angle = 0;
float initial_fov = 45;

float speed = 3.0;
float mouse_speed = 0.005;

int main() {
   if(!glfwInit()) {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      return -1;
   }

   glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow* main_window;

   if(!(main_window = glfwCreateWindow(800, 600, "HW01", NULL, NULL))) {
      std::cerr << "Failed to open GLFW window." << std::endl;
      glfwTerminate();
      return -1;
   }

   glfwMakeContextCurrent(main_window);

   glewExperimental = GL_TRUE;
   GLenum err = glewInit();
   if (err != GLEW_OK) {
      std::cerr << "Failed to initialize GLEW: "
                << glewGetErrorString(err) << std::endl;
      return -1;
   }

   glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);

   GLuint vertex_array_id;
   glGenVertexArrays(1, &vertex_array_id);
   glBindVertexArray(vertex_array_id);

   GLuint program_id = load_shaders("simplevertexshader.vert",
                                    "simplefragmentshader.frag");

   GLuint matrix_id = glGetUniformLocation(program_id, "MVP");
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = { 
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

   do {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(program_id);

      //computeMatricesFromInputs();
      //glm::mat4 projection = getProjectionMatrix();
      //glm::mat4 view = getViewMatrix();
      //glm::mat4 model(1.0f);
      //glm::mat4 mvp = projection * view * model;

      //glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);
      
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

      glfwSwapBuffers(main_window);
      glfwPollEvents();
   } while(glfwGetKey(main_window, GLFW_KEY_ESCAPE) != GLFW_PRESS
          && !glfwWindowShouldClose(main_window));

   glfwTerminate();

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
   glDeleteProgram(program_id);
   glDeleteVertexArrays(1, &vertex_array_id);

   return 0;
}
