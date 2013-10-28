CXX=clang++
CFLAGS=-g -std=c++11
#CFLAGS=-O2 -std=c++11

hw01: main.cpp gl_context.cpp camera.cpp model.cpp
	$(CXX) $(CFLAGS) -lGL -lGLEW -lglfw $^ -o hw01
