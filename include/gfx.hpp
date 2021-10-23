#ifndef _GFX_H_
#define _GFX_H_

#include "../include/chip8hw.hpp"
#include <GL/glut.h>

#ifndef CHIP8_DISPLAY_X
#define CHIP8_DISPLAY_X 64
#endif
#ifndef CHIP8_DISPLAY_Y
#define CHIP8_DISPLAY_Y 32
#endif

extern int modifier, display_width, display_height;

void setupTexture(void);
void updateTexture(void);
void drawTextures(void);
void loadGameCallback(string game);
void keyboardDown(u_int8_t key, int x, int y);
void keyboardUp(u_int8_t key, int x, int y);
void reshape_window(GLsizei w, GLsizei h);

#endif

