ENGINE_SRC_DIR=./src/
ENGINE_INCLUDE_DIR=./include/
GLM_INCLUDE_DIR=glm/
CC=g++
CC_ARGS=-std=gnu++11 -g
OUTPUT_EXECUTABLE=fission
LINKER_FLAGS=-lGL -lm `/usr/local/bin/sdl2-config --cflags --libs` -pthread -lGLEW -lGLU

all: engine

engine:
	mkdir -p build
	 $(CC) $(CC_ARGS) $(ENGINE_SRC_DIR)*.cpp -I$(ENGINE_INCLUDE_DIR) -I$(GLM_INCLUDE_DIR) $(LINKER_FLAGS) -o build/$(OUTPUT_EXECUTABLE)

clean:
	-rm -rf build/
