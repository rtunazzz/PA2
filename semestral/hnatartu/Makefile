# -*- MakeFile -*-
CXX=g++
SOURCEDIR = ./src
CXXFLAGS = -std=c++17 -I$(SOURCEDIR) -Wall -pedantic -g
CPPFILES = $(shell find $(SOURCEDIR) -type f -name '*.cpp')
OBJECTS = $(CPPFILES:.cpp=.o)

all: compile doc
compile: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -lncurses $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

run: compile
	./compile

doc:
	doxygen documentation

.PHONY: clean

clean:
	rm -f $(OBJECTS) ./compile
