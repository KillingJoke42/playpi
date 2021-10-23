#include "../include/gfx.hpp"
#include <cstdlib>
#include <GL/glut.h>
#include "../include/chip8hw.hpp"

using namespace std;

chip8hw c8;
int modifier = 10;
int display_width = CHIP8_DISPLAY_X * modifier;
int display_height = CHIP8_DISPLAY_Y * modifier;
u_int8_t screenData[CHIP8_DISPLAY_X][CHIP8_DISPLAY_Y][3];

void setupTexture(void) {
    for (int i = 0; i < CHIP8_DISPLAY_X; i++) {
        for (int j = 0; j < CHIP8_DISPLAY_Y; j++) {
            screenData[i][j][0] = 0;
            screenData[i][j][1] = 0;
            screenData[i][j][2] = 0;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 
                 0, 
                 3, 
                 CHIP8_DISPLAY_X,
                 CHIP8_DISPLAY_Y, 
                 0, 
                 GL_RGB, 
                 GL_UNSIGNED_BYTE,
                 (GLvoid*) screenData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glEnable(GL_TEXTURE_2D);
}

void updateTexture(void) {
    for (int i = 0; i < CHIP8_DISPLAY_X; i++)
        for (int j = 0; j < CHIP8_DISPLAY_Y; j++)
                screenData[i][j][0] = screenData[i][j][1] = screenData[i][j][2] = 
                    (c8.disp[(j * 64) + i] == 1) ? 255 : 0;

    glTexSubImage2D(GL_TEXTURE_2D, 
					0, 
					0, 
					0, 
					CHIP8_DISPLAY_X, 
					CHIP8_DISPLAY_Y, 
					GL_RGB, 
					GL_UNSIGNED_BYTE, 
					(GLvoid*)screenData);

	glBegin( GL_QUADS );
		glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,			  0.0);
		glTexCoord2d(1.0, 0.0); 	glVertex2d(display_width, 0.0);
		glTexCoord2d(1.0, 1.0); 	glVertex2d(display_width, display_height);
		glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0,			  display_height);
	glEnd();
}

void drawTextures() {
    c8.emulateCycle();

    if (c8.startDraw) {
        glClear(GL_COLOR_BUFFER_BIT);
        updateTexture();
        glutSwapBuffers();
        c8.startDraw = false;
    }
}

void loadGameCallback(string game) {
	c8.loadGame(game);
	return;
}

void keyboardDown(u_int8_t key, int x, int y) {
	c8.setKeys(key, x, y);
}

void keyboardUp(u_int8_t key, int x, int y) {
	c8.resetKeys(key, x, y);
}

void reshape_window(GLsizei w, GLsizei h) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);

	display_height = h;
	display_width = w;
}