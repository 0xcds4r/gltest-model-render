TARGET = builds/opengl_app

CXX = g++
CXXFLAGS = -Wall -std=c++11 -g -Iexternal/tinyobjloader

LIBS = -lGL -lGLEW -lglfw -lassimp

SRC_DIRS := $(shell find . -name "*.cpp")

OBJS = $(SRC_DIRS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
