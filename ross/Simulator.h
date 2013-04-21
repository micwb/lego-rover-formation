#ifndef ROSS_SIMULATOR_H
#define ROSS_SIMULATOR_H

#include "Environment.h"

// global constants
extern const GLint INIT_WINDOW_POSITION[2];
extern const GLint DT;

// global variables
extern GLint       windowSize[2];
extern GLfloat     windowWidth;
extern GLfloat     windowHeight;

// function prototypes
class GLWindow;
void display(GLWindow* glwindow);
void initConsole();
void initWindow();
void keyboardPress(unsigned char keyPressed, GLint mouseX, GLint mouseY);
void keyboardPressSpecial(GLint keyPressed, GLint mouseX, GLint mouseY);
void mouseClick(GLint mouseButton, GLint mouseState,
                GLint mouseX,      GLint mouseY);
void mouseDrag(GLint mouseX, GLint mouseY);
void resizeWindow(GLsizei w, GLsizei h);
void timerFunction(GLint value);

// <test formation functions>
bool     changeFormation(const GLint index);
GLfloat  line(const GLfloat x);
GLfloat  x(const GLfloat x);
GLfloat  absX(const GLfloat x);
GLfloat  negHalfX(const GLfloat x);
GLfloat  negAbsHalfX(const GLfloat x);
GLfloat  negAbsX(const GLfloat x);
GLfloat  parabola(const GLfloat x);
GLfloat  cubic(const GLfloat x);
GLfloat  condSqrt(const GLfloat x);
GLfloat  sine(const GLfloat x);
GLfloat  xRoot3(const GLfloat x);
GLfloat  negXRoot3(const GLfloat x);
extern Function formations[];

// global simulation constants
extern const GLfloat   SELECT_RADIUS;
extern const GLint     N_CELLS;
extern const GLint     MIDDLE_CELL;
extern const Formation DEFAULT_FORMATION;

// global simulation variables
extern Environment env;
extern GLfloat     fRadius;
extern GLint       sID;
extern GLint       fID;
extern GLfloat     fHeading;
extern GLint       fIndex;
extern GLint       selectedIndex;

#endif/*ROSS_SIMULATOR_H*/