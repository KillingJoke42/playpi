#include <cstdlib>
#include <iostream>
#include "../include/chip8hw.hpp"
#include "../include/gfx.hpp"

using namespace std;

int main(int argc, char **argv) {
    loadGameCallback("/home/anantraina/roms/chip8/pong.rom");

    glutInit(&argc, argv);
    glutInitWindowSize(CHIP8_DISPLAY_X, CHIP8_DISPLAY_Y);
    glutInitWindowPosition(320, 320);
    glutCreateWindow("CHIP8");

    glutDisplayFunc(drawTextures);
    glutIdleFunc(drawTextures);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardFunc(keyboardUp);

    setupTexture();

    glutMainLoop();
}