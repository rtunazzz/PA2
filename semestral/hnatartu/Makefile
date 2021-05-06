CXX = g++
CXX_FLAGS = -std=c++17 -Wall -pedantic -g

BIN = bin
SRC = src
INCLUDE_PATHS = include
LIBRARY_PATHS = lib

LIBRARIES = -lSDL2
EXECUTABLE = main

.PHONY: clean all

all:	clean	$(BIN)/$(EXECUTABLE)	doc
	./$(BIN)/$(EXECUTABLE)

dev:	$(BIN)/$(EXECUTABLE)
	./$(BIN)/$(EXECUTABLE)

run:	clean	all

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@ -I$(INCLUDE_PATHS) -L$(LIBRARY_PATHS) $(LIBRARIES)

doc:
	doxygen documentation

clean:
	rm -rf $(BIN)/*
