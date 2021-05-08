CXX = g++
CXX_FLAGS = -std=c++17 -Wall -pedantic -g

BIN = .
SRC = src
INCLUDE_PATHS = assets/include
LIBRARY_PATHS = assets/lib

LIBRARIES = -lSDL2
EXECUTABLE = main

.PHONY: clean all doc dev

all:	compile	doc

compile:	$(BIN)/$(EXECUTABLE)

run:
	./$(BIN)/$(EXECUTABLE)

dev:	compile	run

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@ -I$(INCLUDE_PATHS) -L$(LIBRARY_PATHS) $(LIBRARIES)

doc:
	doxygen Doxyfile

clean:
	rm -rf main main.dsYM doc/*
