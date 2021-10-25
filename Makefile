CC=g++
LDFLAGS=-lglut -lGL -lGLU -lglut

BLD_PATH=./build
SRC_PATH=./src
INCL_PATH=./include

SRC=$(SRC_PATH)/main.cpp $(SRC_PATH)/chip8hw.cpp

build_chip8: $(SRC)
	mkdir -p build && $(CC) $(SRC) $(LDFLAGS) -o $(BLD_PATH)/chip8