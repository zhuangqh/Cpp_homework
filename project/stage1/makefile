all: main
CC = g++
CXX = -c
DEBUG = -g
OP = -Wall -O0
CXX_FLAGS = $(CC) $(DEBUG) $(OP)
#List of Targets
main : Matrix.o Vector.o main.cpp
	$(CXX_FLAGS)	Matrix.o Vector.o main.cpp -o main
Matrix.o : Matrix.cpp Matrix.h
	$(CC) $(CXX)	Matrix.cpp Matrix.h
Vector.o : Vector.cpp Vector.h
	$(CC) $(CXX)	Vector.cpp Vector.h
clean :
	rm -rf *.o *.gch main
