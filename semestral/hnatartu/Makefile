CXX = g++
CXX_FLAGS = -std=c++17 -Wall -pedantic -g -fsanitize=address
ASAN_OPTIONS = 
BIN = .
SRC = src
INCLUDE_PATHS = assets/include
LIBRARY_PATHS = assets/lib

SOURCES = $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/*/*.cpp)

LIBRARIES = -lSDL2 -lSDL2_ttf
EXECUTABLE = main

.PHONY: clean all doc dev memdebug

all:	compile	doc

compile:	$(BIN)/$(EXECUTABLE)

run:
	$(ASAN_OPTIONS) ./$(BIN)/$(EXECUTABLE)

dev:	compile	run

memdebug: ASAN_OPTIONS=detect_leaks=1,abort_on_error=1
memdebug:	dev

$(BIN)/$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXX_FLAGS) $^ -o $@ -I$(INCLUDE_PATHS) -L$(LIBRARY_PATHS) $(LIBRARIES)

doc:
	doxygen Doxyfile

clean:
	rm -rf main main.dsYM doc/*
