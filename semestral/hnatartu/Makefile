CXX = g++
CXX_FLAGS = -std=c++17 -Wall -pedantic -g

BIN = bin# TODO replace this with '.' to comply with the specification
SRC = src
INCLUDE_PATHS = include
LIBRARY_PATHS = lib

LIBRARIES = -lSDL2
EXECUTABLE = main

.PHONY: clean all

all:	clean	$(BIN)/$(EXECUTABLE)	doc
	./$(BIN)/$(EXECUTABLE)

compile:	$(BIN)/$(EXECUTABLE)

run:
	./$(BIN)/$(EXECUTABLE)

dev:	compile	run

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@ -I$(INCLUDE_PATHS) -L$(LIBRARY_PATHS) $(LIBRARIES)

doc:
	doxygen documentation

clean:
	rm -rf $(BIN)/* doc/*
