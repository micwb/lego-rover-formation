//
// Filename:        "Simulator.cpp"
//
// Programmer:      Ross Mead
// Last modified:   07Jan2007
//
// Description:     This program tests the robot cell simulator.
//

// preprocessor directives
#include "Environment.h"
#include "Simulator.h"
#include "../formationcontrol/GLWindow.h"
using namespace std;



// global constants
const GLint INIT_WINDOW_POSITION[2] = {0, 0};       // window offset
const GLint DT                      = 50;           // number of milliseconds

// global variables
GLint       windowSize[2]           = {640, 480};   // window size in pixels
GLfloat     windowWidth             = 4.0f;         // resized window width
GLfloat     windowHeight            = 3.0f;         // resized window height

Function formations[] = {line,        x,       absX,     negHalfX,
                         negAbsHalfX, negAbsX, parabola, cubic,
                         condSqrt,    sine,    xRoot3,   negXRoot3};

// global simulation constants
const GLfloat   SELECT_RADIUS     = 1.5f * DEFAULT_ROBOT_RADIUS;
const GLint     N_CELLS           = 4;
const GLint     MIDDLE_CELL       = 0;//N_CELLS == 1 ? 1 : (N_CELLS - 1) / 2;
const Formation DEFAULT_FORMATION = Formation(formations[0],
                                              DEFAULT_ROBOT_RADIUS *
                                              FACTOR_COLLISION_RADIUS,
                                              Vector(), MIDDLE_CELL, 0,
                                              90.0f);

// global simulation variables
//Environment env(N_CELLS,    DEFAULT_FORMATION);
Environment env;
GLfloat     fRadius       = DEFAULT_FORMATION.getRadius();
GLint       sID           = DEFAULT_FORMATION.getSeedID();
GLint       fID           = DEFAULT_FORMATION.getFormationID();
GLfloat     fHeading      = DEFAULT_FORMATION.getHeading();
GLint       fIndex        = 0;
GLint       selectedIndex = sID;



//
// GLint main(argc, argv)
// Last modified:   04Sep2006
//
// Uses the OpenGL Utility Toolkit to set the
// window up to display the window and its contents.
//
// Returns:     the result of the OpenGL main loop
// Parameters:
//      argc    in      an argument counter
//      argv    in      initialization arguments
//
#if 0
GLint main(GLint argc, char **argv)
{

	// use the GLUT utility to initialize the window, to handle
    // the input and to interact with the windows system
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(windowSize[0], windowSize[1]);
    glutInitWindowPosition(INIT_WINDOW_POSITION[0], INIT_WINDOW_POSITION[1]);
    glutCreateWindow("Simulator");

	// specify the resizing, refreshing, and interactive routines
	glutDisplayFunc(display);
    glutIdleFunc(display);
	glutKeyboardFunc(keyboardPress);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseDrag );
	glutReshapeFunc(resizeWindow);
    glutSpecialFunc(keyboardPressSpecial);
	glutTimerFunc(50, timerFunction, 1);

    // initialize and execute the robot cell environment
    initWindow();
    initConsole();
    return glutMainLoop();
}   // main(GLint, char **)
#endif


//
// bool changeFormation(index)
// Last modified: 04Sep2006
//
// Attempts to change the current formation,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      index   in      the index of the formation to change to
//
bool changeFormation(const GLint index)
{
    fIndex = index;

    // determine if a new seed has been selected
    if (selectedIndex != -1)
    {
		Cell* pCell = env.getCell(sID);
		if (pCell)
			pCell->setColor(DEFAULT_CELL_COLOR);
        sID = selectedIndex;
    }

    // send the new formation definition to the seed
    return env.sendMsg(new Formation(formations[index], fRadius, Vector(),
                                     sID,             ++fID,     fHeading),
                       sID, ID_OPERATOR, CHANGE_FORMATION);
}   // changeFormation(const GLint)



//
// void display()
// Last modified:   04Sep2006
//
// Clears the frame buffer and draws the simulated cells within the window.
//
// Returns:     <none>
// Parameters:  <none>
//
void display(GLWindow* glwindow)
{
	glClear(GL_COLOR_BUFFER_BIT);   // clear background color
    glMatrixMode(GL_MODELVIEW);     // modeling transformation

    // draws environment robot cells
	if (env.getCell(sID))
		env.getCell(sID)->setColor(GREEN);
    env.draw();

	glFlush();                      // force the execution of OpenGL commands
}   // display()



//
// void initConsole()
// Last modified: 22Sep2006
//
// Initializes the console to read a the following program description:
//
// ******************************************
// *                                        *
// * ALGORITHMS FOR CONTROL AND INTERACTION *
// *     OF LARGE FORMATIONS OF ROBOTS      *
// *                                        *
// *   Ross Mead & Dr. Jerry B. Weinberg    *
// *  Southern Illinois Univ. Edwardsville  *
// *                                        *
// ******************************************
//
// Use the 'ARROW' keys to translate and rotate the seed robot.
//
// Use the '+/-' keys to increase/decrease the separation between robots.
//
// Use the '</>' keys to rotate the robot headings relative to the formation.
//
// Use the '0-9' keys to change to a formation seeded at the selected robot.
//
// PRESET FORMATIONS
// -----------------
// 0) f(x) = 0
// 1) f(x) = x
// 2) f(x) = |x|
// 3) f(x) = -0.5 x
// 4) f(x) = -|0.5 x|
// 5) f(x) = -|x|
// 6) f(x) = x^2
// 7) f(x) = x^3
// 8) f(x) = {sqrt(x),  x >= 0 | -sqrt|x|, x < 0}
// 9) f(x) = 0.05 sin(10 x)
//
// Use the mouse to select a robot.
//
// Use 'h|l|p|t' to toggle robot display settings.
//
// Returns:     <none>
// Parameters:  <none>
//
void initConsole()
{
    cout << "******************************************"      << endl
         << "*                                        *"      << endl
         << "* ALGORITHMS FOR CONTROL AND INTERACTION *"      << endl
         << "*     OF LARGE FORMATIONS OF ROBOTS      *"      << endl
         << "*                                        *"      << endl
         << "*   Ross Mead & Dr. Jerry B. Weinberg    *"      << endl
         << "*  Southern Illinois Univ. Edwardsville  *"      << endl
         << "*                                        *"      << endl
         << "******************************************"      << endl << endl
         << "Use the 'ARROW' keys to "
         << "translate and rotate the seed robot."
         << endl << endl
         << "Use the '+/-' keys to "
         << "increase/decrease the separation between robots."
         << endl << endl
         << "Use the '</>' keys to "
         << "rotate the robot headings relative to the formation."
         << endl << endl
         << "Use the '0-9' keys to "
         << "change to a formation seeded at the selected robot."
         << endl << endl
         << "PRESET FORMATIONS\n-----------------"            << endl
         << "0) f(x) = 0"                                     << endl
         << "1) f(x) = x"                                     << endl
         << "2) f(x) = |x|"                                   << endl
         << "3) f(x) = -0.5 x"                                << endl
         << "4) f(x) = -|0.5 x|"                              << endl
         << "5) f(x) = -|x|"                                  << endl
         << "6) f(x) = x^2"                                   << endl
         << "7) f(x) = x^3"                                   << endl
         << "8) f(x) = {sqrt(x),  x >= 0 | -sqrt|x|, x < 0}"  << endl
         << "9) f(x) = 0.05 sin(10 x)"                        << endl << endl
         << "Use the mouse to select a robot."                << endl << endl
         << "Use 'h|l|p|t' to toggle robot display settings." << endl << endl;
}   // initConsole()



//
// void initWindow()
// Last modified:   04Sep2006
//
// Initializes the simulator window.
//
// Returns:     <none>
// Parameters:  <none>
//
void initWindow()
{

    // clear background color
    glClearColor(env.color[0], env.color[1], env.color[2], 0.0f);
   	
    glViewport(0, 0, windowSize[0], windowSize[1]); // viewport transformation

    glMatrixMode(GL_PROJECTION);    // projection transformation
    glLoadIdentity();               // initialize projection identity matrix
}   // initWindow()



//
// void keyboardPress(keyPressed, mouseX, mouseY)
// Last modified:   04Sep2006
//
// Handles the keyboard input (ASCII Characters).
//
// Returns:     <none>
// Parameters:
//      keyPressed  in      the ASCII key that was pressed
//      mouseX      in      the x-coordinate of the mouse position
//      mouseY      in      the y-coordinate of the mouse position
//
void keyboardPress(unsigned char keyPressed, GLint mouseX, GLint mouseY)
{
	Cell* pCell = env.getCell(sID);
    if ((keyPressed >= '0') && (keyPressed <= '9'))
    {
        char cIndex = keyPressed;
        changeFormation(atoi(&cIndex));
    }
    else switch (keyPressed)
	{

        // change formation heading
        case '<': case ',':
            fHeading += 1.0f;
            changeFormation(fIndex);
			if (pCell)
				pCell->rotateRelative(
					min(1.0f, pCell->maxAngSpeed()));
            break;
        case '>': case '.':
            fHeading -= 1.0f;
            changeFormation(fIndex);
			if (pCell)
				pCell->rotateRelative(
					-min(1.0f, pCell->maxAngSpeed()));
            break;

        // change formation scale
        case '+': case '=':
            fRadius += 0.01f;
            changeFormation(fIndex);
            break;
        case '-': case '_':
            fRadius -= 0.01f;
			if (pCell)
				fRadius  = max(fRadius, pCell->collisionRadius());
            changeFormation(fIndex);
            break;

        // show position
        case 'h': case 'H':
			if (pCell)
				env.showHeading(!pCell->showHeading);   break;
        case 'l': case 'L':
			if (pCell)
				env.showLine(!pCell->heading.showLine); break;
        case 'p': case 'P':
			if (pCell)
				env.showPos(!pCell->showPos);           break;
        case 't': case 'T':
			if (pCell)
				env.showHead(!pCell->heading.showHead); break;
    }
}   // keyboardPress(unsigned char, GLint, GLint)



//
// void keyboardPressSpecial(keyPressed, mouseX, mouseY)
// Last modified:   18May2006
//
// Handles the keyboard input (non-ASCII Characters).
//
// Returns:     <none>
// Parameters:
//      keyPressed  in      the non-ASCII key that was pressed
//      mouseX      in      the x-coordinate of the mouse position
//      mouseY      in      the y-coordinate of the mouse position
//
void keyboardPressSpecial(GLint keyPressed, GLint mouseX, GLint mouseY)
{
	Cell* pCell = env.getCell(sID);
    switch (keyPressed)
    {
        case GLUT_KEY_LEFT:
			if (pCell)
				pCell->rotateRelative(
					min(1.0f, pCell->maxAngSpeed()));
            break;
        case GLUT_KEY_UP:
			if (pCell)
				pCell->translateRelative(
					min(0.001f, pCell->maxSpeed()));
            break;
        case GLUT_KEY_RIGHT:
			if (pCell)
				pCell->rotateRelative(
					-min(1.0f, pCell->maxAngSpeed()));
            break;
        case GLUT_KEY_DOWN:
			if (pCell)
				pCell->translateRelative(
					-min(0.001f, pCell->maxSpeed()));
            break;
        default: break;
    }
}   // keyboardPressSpecial(GLint, GLint, GLint)



//
// void mouseClick(mouseButton, mouseState, mouseX, mouseY)
// Last modified:   03Nov2006
//
// Reacts to mouse clicks.
//
// Returns:     <none>
// Parameters:
//      mouseButton     in      the mouse button that was pressed
//      mouseState      in      the state of the mouse
//      mouseX          in      the x-coordinate of the mouse position
//      mouseY          in      the y-coordinate of the mouse position
//
void mouseClick(GLint mouseButton,    GLint mouseState,
                GLint mouseX, GLint mouseY)
{
    if (mouseState == GLUT_DOWN)
    {
	    GLfloat x     = windowWidth * mouseX / windowSize[0] -
                        0.5 * windowWidth;
	    GLfloat y     = 0.5 * windowHeight -
                        (windowHeight * mouseY / windowSize[1]);
        selectedIndex = sID;
        for (GLint i = 0; i < env.getNCells(); ++i)
            if ((selectedIndex == sID) && env.getCell(i) &&
                (sqrt(pow(env.getCell(i)->x - x, 2.0f)  +
                      pow(env.getCell(i)->y - y, 2.0f)) < SELECT_RADIUS))
                    env.getCell(selectedIndex = i)->setColor(RED);
                else if ( (i != sID) && env.getCell(i) )
                    env.getCell(i)->setColor(DEFAULT_CELL_COLOR);
    }
	glutPostRedisplay();            // redraw the scene
}   // mouseClick(GLint, GLint, GLint, GLint)



//
// void mouseDrag(mouseX, mouseY)
// Last modified:   04Sep2006
//
// Moves the position of a nearby cell to the
// current mouse pointer position if the mouse
// button is pressed and the pointer is in motion.
//
// Returns:     <none>
// Parameters:
//      mouseX  in      the x-coordinate of the mouse position
//      mouseY  in      the y-coordinate of the mouse position
//
void mouseDrag(GLint mouseX, GLint mouseY)
{
    if ( (selectedIndex != ID_NO_NBR) && (env.getCell(selectedIndex)) )
        env.getCell(selectedIndex)->set(
            windowWidth * mouseX / windowSize[0] - 0.5 * windowWidth,
	        0.5 * windowHeight - (windowHeight * mouseY / windowSize[1]));
	glutPostRedisplay();            // redraw the scene
}   // mouseDrag(GLint, GLint)



//
// void resizeWindow(w, h)
// Last modified:   04Sep2006
//
// Scales the rendered scene according to the window dimensions,
// setting the global variables so the mouse operations will
// correspond to mouse pointer positions.
//
// Returns:     <none>
// Parameters:
//      w       in      the new screen width
//      h       in      the new screen height
//
void resizeWindow(GLsizei w, GLsizei h)
{
	windowSize[0] = w;              // obtain new screen width
	windowSize[1] = h;              // obtain new screen height

    glClear(GL_COLOR_BUFFER_BIT);   // clear color buffer to draw next frame
	glViewport(0, 0, w, h);         // viewport transformation

    glMatrixMode(GL_PROJECTION);    // projection transformation
    glLoadIdentity();               // initialize projection identity matrix

    if (w <= h)
	{
		windowWidth  = 2.0f;
		windowHeight = 2.0f * (GLfloat)h / (GLfloat)w;
        glOrtho(-1.0f, 1.0f, -1.0f * (GLfloat)h / (GLfloat)w,
		        (GLfloat)h / (GLfloat)w, -10.0f, 10.0f);
	}
    else
	{
		windowWidth  = 2.0f * (GLfloat)w / (GLfloat)h;
		windowHeight = 2.0f;
        glOrtho(-1.0f * (GLfloat)w / (GLfloat)h,
		        (GLfloat)w / (GLfloat)h,
				-1.0f, 1.0f, -10.0f, 10.0f);
	}
    //glutPostRedisplay();            // redraw the scene
}   // resizeWindow(GLsizei, GLsizei)



//
// void timerFunction(value)
// Last modified:   04Sep2006
//
// Updates the environment and redraws.
//
// Returns:     <none>
// Parameters:
//      value   in      the value of the timer
//
void timerFunction(GLint value)
{
    env.step();                     // update the robot cell environment
    
    // force a redraw after a number of milliseconds
	glutPostRedisplay();            // redraw the scene
	glutTimerFunc(DT, timerFunction, 1);
}   // timerFunction(GLint)



// <test formation functions>

//
// GLfloat line(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = 0.
//
// Returns:     f(x) = 0
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat line(const GLfloat x)
{
    return 0.0f;
}   // line(const GLfloat)



//
// GLfloat x(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = x.
//
// Returns:     f(x) = x
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat x(const GLfloat x)
{
    return x;
}   // x(const GLfloat)



//
// GLfloat absX(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = |x|.
//
// Returns:     f(x) = |x|
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat absX(const GLfloat x)
{
    return abs(x);
}   // absX(const GLfloat)



//
// GLfloat negHalfX(x)
// Last modified:   07Jan2007
//
// Returns formation function definition f(x) = -0.5 x.
//
// Returns:     f(x) = -0.5 x
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat negHalfX(const GLfloat x)
{
    return -0.5f * x;
}   // negHalfX(const GLfloat)



//
// GLfloat negAbsHalfX(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = -|0.5 x|.
//
// Returns:     f(x) = -|0.5 x|
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat negAbsHalfX(const GLfloat x)
{
    return -abs(0.5f * x);
}   // negAbsHalfX(const GLfloat)



//
// GLfloat negAbsX(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = -|x|.
//
// Returns:     f(x) = -|x|
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat negAbsX(const GLfloat x)
{
    return -abs(x);
}   // negAbsX(const GLfloat)



//
// GLfloat parabola(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = x^2.
//
// Returns:     f(x) = x^2
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat parabola(const GLfloat x)
{
    return pow(x, 2.0f);
}   // parabola(const GLfloat)



//
// GLfloat cubic(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = x^3.
//
// Returns:     f(x) = x^3
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat cubic(const GLfloat x)
{
    return pow(x, 3.0f);
}   // cubic(const GLfloat)



//
// GLfloat condSqrt(x)
// Last modified:   04Sep2006
//
// Returns formation function definition
// f(x) = {sqrt(x),  x = 0 | -sqrt|x|, x < 0}.
//
// Returns:     f(x) = {sqrt(x),  x = 0 | -sqrt|x|, x < 0}
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat condSqrt(const GLfloat x)
{
    return sqrt(abs(0.5f * x)) * ((x >= 0) ? 1.0f : -1.0f);
}   // condSqrt(const GLfloat)



//
// GLfloat sine(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = 0.05 sin(10 x).
//
// Returns:     f(x) = 0.05 sin(10 x)
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat sine(const GLfloat x)
{
    return 0.05f * sin(10.0f * x);
}   // sine(const GLfloat)



//
// GLfloat xRoot3(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = x sqrt(3).
//
// Returns:     f(x) = x sqrt(3)
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat xRoot3(const GLfloat x)
{
    return x * sqrt(3.0f);
}   // xRoot3(const GLfloat)



//
// GLfloat negXRoot3(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = -x sqrt(3).
//
// Returns:     f(x) = -x sqrt(3)
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat negXRoot3(const GLfloat x)
{
    return -x * sqrt(3.0f);
}   // negXRoot3(const GLfloat)
