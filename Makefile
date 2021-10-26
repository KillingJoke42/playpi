CC=g++
LDFLAGS=-lglut -lGL -lGLU -lglut

BLD_PATH=./build
SRC_PATH=./src
INCL_PATH=./include

SRC=$(SRC_PATH)/main.cpp $(SRC_PATH)/chip8hw.cpp $(SRC_PATH)/simplegui.cpp
BLDGTK=`pkg-config --cflags --libs gtk+-3.0`

build_chip8: $(SRC)
	mkdir -p build && $(CC) $(SRC) $(LDFLAGS) -o $(BLD_PATH)/chip8 $(BLDGTK)