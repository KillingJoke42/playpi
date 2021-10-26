#include <cstdlib>
#include <iostream>
#include <GL/glu.h>
#include <GL/glut.h>
#include "../include/chip8hw.hpp"
#include "../include/simplegui.hpp"

using namespace std;

chip8hw c8;

void display();
void reshape_window(GLsizei w, GLsizei h);
void keyboardUp(unsigned char key, int x, int y);
void keyboardDown(unsigned char key, int x, int y);
void setupTexture();

int modifier = 10;
int display_width = CHIP8_DISPLAY_X * modifier;
int display_height = CHIP8_DISPLAY_Y * modifier;
u_int8_t screenData[CHIP8_DISPLAY_Y][CHIP8_DISPLAY_X][3]; 

void loadGameCallback(string game) {
	c8.loadGame(game);
	return;
}

int main(int argc, char **argv) {
	start_form(argc, argv);
    loadGameCallback(romfile);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(display_width, display_height);
    glutInitWindowPosition(320, 320);
    glutCreateWindow("CHIP8");

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape_window);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);

    setupTexture();

    glutMainLoop();
}

// Setup Texture
void setupTexture()
{
	// Clear screen
	for(int y = 0; y < CHIP8_DISPLAY_Y; ++y)		
		for(int x = 0; x < CHIP8_DISPLAY_X; ++x)
			screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;

	// Create a texture 
	glTexImage2D(GL_TEXTURE_2D, 0, 3, CHIP8_DISPLAY_X, CHIP8_DISPLAY_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);

	// Set up the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 

	// Enable textures
	glEnable(GL_TEXTURE_2D);
}

void updateTexture(const chip8hw& c8)
{	
	// Update pixels
	for(int y = 0; y < CHIP8_DISPLAY_Y; ++y)		
		for(int x = 0; x < CHIP8_DISPLAY_X; ++x)
			if(c8.disp[(y * CHIP8_DISPLAY_X) + x] == 0)
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;	// Disabled
			else 
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 255;  // Enabled
		
	// Update Texture
	glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, CHIP8_DISPLAY_X, CHIP8_DISPLAY_Y, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);

	glBegin( GL_QUADS );
		glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,			  0.0);
		glTexCoord2d(1.0, 0.0); 	glVertex2d(display_width, 0.0);
		glTexCoord2d(1.0, 1.0); 	glVertex2d(display_width, display_height);
		glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0,			  display_height);
	glEnd();
}

// Old gfx code
void drawPixel(int x, int y)
{
	glBegin(GL_QUADS);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + 0.0f,	 0.0f);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + 0.0f,	 0.0f);
	glEnd();
}

void updateQuads(const chip8hw& c8)
{
	// Draw
	for(int y = 0; y < 32; ++y)		
		for(int x = 0; x < 64; ++x)
		{
			if(c8.disp[(y*64) + x] == 0) 
				glColor3f(0.0f,0.0f,0.0f);			
			else 
				glColor3f(1.0f,1.0f,1.0f);

			drawPixel(x, y);
		}
}

void display()
{
	c8.emulateCycle();
		
	if(c8.startDraw)
	{
		// Clear framebuffer
		glClear(GL_COLOR_BUFFER_BIT);
        
		updateTexture(c8);

		// Swap buffers!
		glutSwapBuffers();    

		// Processed frame
		c8.startDraw = false;
	}
}

void reshape_window(GLsizei w, GLsizei h)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);        
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

	// Resize quad
	display_width = w;
	display_height = h;
}

void keyboardDown(unsigned char key, int x, int y)
{
	if(key == 27)    // esc
		exit(0);

	if(key == '1')		c8.key[0x1] = 1;
	else if(key == '2')	c8.key[0x2] = 1;
	else if(key == '3')	c8.key[0x3] = 1;
	else if(key == '4')	c8.key[0xC] = 1;

	else if(key == 'q')	c8.key[0x4] = 1;
	else if(key == 'w')	c8.key[0x5] = 1;
	else if(key == 'e')	c8.key[0x6] = 1;
	else if(key == 'r')	c8.key[0xD] = 1;

	else if(key == 'a')	c8.key[0x7] = 1;
	else if(key == 's')	c8.key[0x8] = 1;
	else if(key == 'd')	c8.key[0x9] = 1;
	else if(key == 'f')	c8.key[0xE] = 1;

	else if(key == 'z')	c8.key[0xA] = 1;
	else if(key == 'x')	c8.key[0x0] = 1;
	else if(key == 'c')	c8.key[0xB] = 1;
	else if(key == 'v')	c8.key[0xF] = 1;

	//printf("Press key %c\n", key);
}

void keyboardUp(unsigned char key, int x, int y)
{
	if(key == '1')		c8.key[0x1] = 0;
	else if(key == '2')	c8.key[0x2] = 0;
	else if(key == '3')	c8.key[0x3] = 0;
	else if(key == '4')	c8.key[0xC] = 0;

	else if(key == 'q')	c8.key[0x4] = 0;
	else if(key == 'w')	c8.key[0x5] = 0;
	else if(key == 'e')	c8.key[0x6] = 0;
	else if(key == 'r')	c8.key[0xD] = 0;

	else if(key == 'a')	c8.key[0x7] = 0;
	else if(key == 's')	c8.key[0x8] = 0;
	else if(key == 'd')	c8.key[0x9] = 0;
	else if(key == 'f')	c8.key[0xE] = 0;

	else if(key == 'z')	c8.key[0xA] = 0;
	else if(key == 'x')	c8.key[0x0] = 0;
	else if(key == 'c')	c8.key[0xB] = 0;
	else if(key == 'v')	c8.key[0xF] = 0;
}