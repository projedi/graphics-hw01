CXX=g++
CFLAGS=-g
#CFLAGS=-O2

hw01: main.cpp
	$(CXX) $(CFLAGS) -lGL -lGLEW -lglfw main.cpp -o hw01
