CXX = g++
CXX_FLAGS = -std=c++17 -Wall -pedantic -g -fsanitize=address

BIN = .
SRC = src

SOURCES = $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/*/*.cpp)

ASAN_OPTIONS = 

SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LDFLAGS = $(shell sdl2-config --libs)

CFLAGS = $(SDL_CFLAGS)
LDFLAGS = $(SDL_LDFLAGS) -lSDL2_ttf

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
	$(CXX) $(CXX_FLAGS) $(CFLAGS) $(LDFLAGS) $^ -o $@

doc:
	doxygen Doxyfile

clean:
	rm -rf main main.dsYM doc/*
