//
// Filename:        "Color.h"
//
// Programmer:      Ross Mead
// Last modified:   18Jul2006
//
// Description:     This enumerated type defines a color.
//

// preprocessor directives
#ifndef COLOR_H
#define COLOR_H
#include "../GL/glut.h"
using namespace std;

// predefined colors
enum  Color  {INVISIBLE,                    // color index values
              BLACK, WHITE, RED,  YELLOW,
              GREEN, CYAN,  BLUE, MAGENTA};
const GLint   N_COLORS           = 9;       // number of colors
const GLfloat COLOR[N_COLORS][3] = {{-1.0f, -1.0f, -1.0f},
									{ 0.0f,  0.0f,  0.0f},
                                    { 1.0f,  1.0f,  1.0f},
									{ 1.0f,  0.0f,  0.0f},
									{ 1.0f,  1.0f,  0.0f},
									{ 0.0f,  1.0f,  0.0f},
									{ 0.0f,  1.0f,  1.0f},
									{ 0.0f,  0.0f,  1.0f},
									{ 1.0f,  0.0f,  1.0f}};
#endif
