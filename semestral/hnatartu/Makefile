CXX = g++
CXX_FLAGS = -std=c++17 -Wall -pedantic -g

BIN = .
SRC = src
INCLUDE_PATHS = assets/include
LIBRARY_PATHS = assets/lib

SOURCES = $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/*/*.cpp)

LIBRARIES = -lSDL2 -lSDL2_ttf
EXECUTABLE = main

.PHONY: clean all doc dev

all:	compile	doc

compile:	$(BIN)/$(EXECUTABLE)

run:
	./$(BIN)/$(EXECUTABLE)

dev:	compile	run

$(BIN)/$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXX_FLAGS) $^ -o $@ -I$(INCLUDE_PATHS) -L$(LIBRARY_PATHS) $(LIBRARIES)

doc:
	doxygen Doxyfile

clean:
	rm -rf main main.dsYM doc/*
