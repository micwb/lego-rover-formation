//
// Filename:        "Environment.cpp"
//
// Programmer:      Ross Mead
// Last modified:   07Sep2006
//
// Description:     This class implements a robot cell environment.
//

// preprocessor directives
#include "Environment.h"
#include <time.h>
#include "../formationcontrol/helpers.h"
#include "simulator.h"


// <constructors>

//
// Environment(n, f, colorIndex)
// Last modified: 04Sep2006
//
// Default constructor that initializes
// this environment to the parameterized values.
//
// Returns:     <none>
// Parameters:
//      n           in      the initial number of cells
//      f           in      the initial formation
//      colorIndex  in      the initial array index of the color of the cells
//

#if 0
Environment::Environment(const GLint     n,
                         const Formation f,
                         const Color     colorIndex)
{
    if (!init(n, f, colorIndex)) clear();
}   // Environment(const GLint, const Formation, const Color)
#endif


//
// Environment(e)
// Last modified: 27Aug2006
//
// Copy constructor that copies the contents of
// the parameterized environment into this environment.
//
// Returns:     <none>
// Parameters:
//      e       in/out      the environment being copied
//
Environment::Environment(const Environment &e)
    : cells(e.cells), msgQueue(e.msgQueue)
{
}   // Environment(const Environment &)



// <destructors>

//
// ~Environment()
// Last modified: 27Aug2006
//
// Destructor that clears this environment.
//
// Returns:     <none>
// Parameters:  <none>
//
Environment::~Environment()
{
    clear();
}   // ~Environment()



// <virtual public mutator functions>

//
// bool setColor(r, g, b)
// Last modified: 03Sep2006
//
// Attempts to set the color to the parameterized RGB color values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      r       in/out  the red in the color to be set to
//      g       in/out  the green in the color to be set to
//      b       in/out  the blue in the color to be set to
//
bool Environment::setColor(const GLfloat r, const GLfloat g, const GLfloat b)
{
    color[GLUT_RED]   = r;
    color[GLUT_GREEN] = g;
    color[GLUT_BLUE]  = b;
    return true;
}   // setColor(const GLfloat, const GLfloat, const GLfloat)



//
// bool setColor(clr)
// Last modified: 03Sep2006
//
// Attempts to set the color to the parameterized RGB color values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      clr     in/out  the color to be set to
//
bool Environment::setColor(const GLfloat clr[3])
{
    return setColor(clr[GLUT_RED], clr[GLUT_GREEN], clr[GLUT_BLUE]);
}   // setColor(const GLfloat [])



//
// bool setColor(colorIndex)
// Last modified: 27Aug2006
//
// Attempts to set the color to the parameterized RGB color values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      colorIndex  in/out  the index of the color to be set to
//
bool Environment::setColor(const Color colorIndex)
{
    return setColor(COLOR[(GLint)colorIndex]);
}   // setColor(const Color)

bool Environment::addCell(int xi, int yi, float heading, Cell *c)
{
    if ((c == NULL) && ((c = new Cell()) == NULL)) return false;
    // assign x-/y-position
    i2f(&(c->x), &(c->y), xi, yi, windowSize[0], windowSize[1]);
    c->setHeading(heading);
	c->setColor(DEFAULT_ROBOT_COLOR);
    c->setEnvironment(this);

    // attempt to add this cell to the cell list
    if (!cells.insertTail(c))
    {
        delete c;
        return false;
    }
    return true;
}


// <public mutator functions>

//
// bool addCell(c)
// Last modified: 27Aug2006
//
// Attempts to add a cell to the environment,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      c       in      the cell being added
//
bool Environment::addCell(Cell *c)
{
    if ((c == NULL) && ((c = new Cell()) == NULL)) return false;

    // assign random x-/y-position, making sure that no cells are overlapping
    bool done = true;
    do
    {
        c->x = frand(-1.0f, 1.0f);
        c->y = frand(-1.0f, 1.0f);
        done = true;
        for (int i = 0; i < cells.getSize(); ++i)
            if (c->getDistanceTo(*cells[i]) <= c->collisionRadius())
                done = false;
    }   while (!done);
    c->setHeading(frand(-180.0f, 180.0f));  // assign random heading
    c->setEnvironment(this);

    // attempt to add this cell to the cell list
    if (!cells.insertTail(c))
    {
        delete c;
        return false;
    }
    return true;
}   // addCell(Cell *)



//
// bool removeCell()
// Last modified: 27Aug2006
//
// Attempts to remove a cell from the environment,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
bool Environment::removeCell()
{
    Cell *c = NULL;
    if (!removeCell(c)) return false;
    delete c;
    return true;
}   // removeCell()



//
// bool removeCell(c)
// Last modified: 27Aug2006
//
// Attempts to remove a cell from the environment,
// storing the address of the removed cell and
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      c       in/out  set to the cell being removed
//
bool Environment::removeCell(Cell* &c)
{
    return cells.removeTail(c);
}   // removeCell(Cell* &)



// <public accessor functions>

//
// Cell* getCell() const
// Last modified: 27Aug2006
//
// Returns the cell at the parameterized position.
//
// Returns:     the cell at the parameterized position
// Parameters:  <none>
//
Cell* Environment::getCell(GLint pos) const
{
    Cell *head = NULL;
    if ((pos < 0) || (pos > getNCells()) || (!cells.getHead(head)))
        return NULL;
    pos -= head->getID();
    return cells[(pos < 0) ? getNCells() + pos : pos];
}   // getCell(GLint) const



//
// LinkedList<Cell *> getCells() const
// Last modified: 27Aug2006
//
// Returns all of the cells in the environment.
//
// Returns:     all of the cells in the environment
// Parameters:  <none>
//
LinkedList<Cell *> Environment::getCells()
{
    return cells;
}   // getCells()



//
// GLint getNCells() const
// Last modified: 27Aug2006
//
// Returns the number of cells in the environment.
//
// Returns:     the number of cells in the environment
// Parameters:  <none>
//
GLint Environment::getNCells() const
{
    return cells.getSize();
}   // getNCells() const



// <virtual public utility functions>

//
// void draw()
// Last modified: 27Aug2006
//
// Renders the environment.
//
// Returns:     <none>
// Parameters:  <none>
//
void Environment::draw()
{
    Cell *currCell = NULL;
    for (GLint i = 0; i < getNCells(); ++i)
        if (cells.getHead(currCell))
        {
            currCell->draw();
            ++cells;
        }
}   // draw()



//
// void step()
// Last modified: 27Aug2006
//
// Executes the next step in each cell in the environment
// and forwards all sent packets to their destinations.
//
// Returns:     <none>
// Parameters:  <none>
//
bool Environment::step()
{
    Cell *currCell = NULL;
    for (GLint i = 0; i < getNCells(); ++i)
    {
        if (!cells.getHead(currCell)) return false;
        currCell->step();
        ++cells;
    }

    // forwards all messages sent via robot cell communication
    return forwardPackets();
}   // step()



//
// void clear()
// Last modified: 27Aug2006
//
// Clears this environment.
//
// Returns:     <none>
// Parameters:  <none>
//
void Environment::clear()
{
    while (removeCell());
}   // clear()



// <public utility functions>

//
// Vector getRelationship(toID, fromID)
// Last modified: 27Aug2006
//
// Returns the relationship between the two cells
// with the parameterized ID's.
//
// Returns:     the relationship between two cells
// Parameters:
//      toID    in      the ID of the cell being related to
//      fromID  in      the ID of the cell being related from
//
Vector Environment::getRelationship(const GLint toID, const GLint fromID)
{
    Cell  *toCell = getCell(toID), *fromCell = getCell(fromID);
    if ((toCell == NULL) || (fromCell == NULL)) return Vector();
    Vector temp   = *toCell - *fromCell;
    temp.rotateRelative(-fromCell->getHeading());
    return temp;
}   // getRelationship(const GLint, const GLint)



//
// bool sendMsg(msg, toID, fromID, type)
// Last modified: 07Sep2006
//
// Attempts to send a packet to its destination
// based upon the given parameters, returning
// true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      msg     in/out  the message being sent
//      toID    in      the ID of the cell receiving the packet
//      fromID  in      the ID of the cell sending the packet
//      type    in      the type of message being sent
//
bool Environment::sendMsg(const Message &msg,
                          const GLint    toID,
                          const GLint    fromID,
                          const GLint    type)
{
    return sendPacket(Packet(msg, toID, fromID, type));
}   // sendMsg(const Message &, const GLint, const GLint, const GLint)



//
// bool sendPacket(p)
// Last modified: 07Sep2006
//
// Attempts to send a packet to its destination,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      p       in/out  the packet being sent
//
bool Environment::sendPacket(const Packet &p)
{

    // discrete message passing
    //if (msgQueue.enqueue(p)) return true;

    // continuous message passing
    if (forwardPacket(p)) return true;

    delete p.msg;
    return false;
}   // sendPacket(const Packet &)



//
// bool forwardPacket(p)
// Last modified: 07Sep2006
//
// Attempts to forward a packet to its destination,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      p       in/out  the packet being forwarded
//
bool Environment::forwardPacket(const Packet &p)
{
    Cell *c = getCell(p.toID);
    if ((c != NULL) && (c->msgQueue.enqueue(p))) return true;
    delete p.msg;
    return false;
}   // forwardPacket(const Packet &)



//
// bool forwardPackets()
// Last modified: 28Aug2006
//
// Attempts to forward all packets to their destinations,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
bool Environment::forwardPackets()
{
    Packet p;
    while (msgQueue.dequeue(p))
        if (!forwardPacket(p)) return false;
    return true;
}   // forwardPackets()



// <public utility cell functions>

//
// bool showLine(show)
// Last modified: 28Aug2006
//
// Attempts to display the line of the heading vector of each cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      show    in      display flag for showing the line
//
bool Environment::showLine(const bool show)
{
    Cell *currCell;
    for (GLint i = 0; i < getNCells(); ++i)
        if (cells.getHead(currCell))
        {
            currCell->heading.showLine = show;
            ++cells;
        }
        else return false;
    return true;
}   // showPos(const bool)



//
// bool showHead(show)
// Last modified: 28Aug2006
//
// Attempts to display the head of the heading vector of each cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      show    in      display flag for showing the head
//
bool Environment::showHead(const bool show)
{
    Cell *currCell;
    for (GLint i = 0; i < getNCells(); ++i)
        if (cells.getHead(currCell))
        {
            currCell->heading.showHead = show;
            ++cells;
        }
        else return false;
    return true;
}   // showHeading(const bool)



//
// bool showPos(show)
// Last modified: 28Aug2006
//
// Attempts to display the position vector of each cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      show    in      display flag for showing the position vector
//
bool Environment::showPos(const bool show)
{
    Cell *currCell;
    for (GLint i = 0; i < getNCells(); ++i)
        if (cells.getHead(currCell))
        {
            currCell->showPos          = show;
            ++cells;
        }
        else return false;
    return true;
}   // showPos(const bool)



//
// bool showHeading(show)
// Last modified: 28Aug2006
//
// Attempts to display the heading vector of each cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      show    in      display flag for showing the heading vector
//
bool Environment::showHeading(const bool show)
{
    Cell *currCell;
    for (GLint i = 0; i < getNCells(); ++i)
        if (cells.getHead(currCell))
        {
            currCell->showHeading      = show;
            ++cells;
        }
        else return false;
    return true;
}   // showHeading(const bool)



// <virtual protected utility functions>

//
// bool init(n, f, colorIndex)
// Last modified: 28Aug2006
//
// Initializes the environment to the parameterized values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      n           in      the initial number of cells
//      f           in      the initial formation
//      colorIndex  in      the initial array index of the color of the cells
//
bool Environment::init(const GLint     n,
                       const Formation f,
                       const Color     colorIndex)
{
    return initCells(n, f) && setColor(colorIndex);
}   // init(const GLint, const Formation, const Color)

extern int* gXPos;
extern int* gYPos;
extern float* gHeading;

bool Environment::areRobotsReady()
{
   return gXPos && gYPos && gHeading;
}

bool Environment::initRobots(const Formation f)
{
	for (GLint i = 0; i < N_CELLS; ++i)
		if (!addCell(gXPos[i], gYPos[i], gHeading[i]))
			return false;

    // initialize each robot's neighborhood
    if (!initNbrs()) return false;

	setColor(BLACK);

    return (getNCells() == N_CELLS) &&
           sendMsg(new Formation(f), f.getSeedID(),
                   ID_OPERATOR,      CHANGE_FORMATION);
}


//
// bool initCells(n, f)
// Last modified: 28Aug2006
//
// Initializes each cell to the parameterized values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      n           in      the initial number of cells
//      f           in      the initial formation
//
bool Environment::initCells(const GLint n, const Formation f)
{
    srand(time(NULL));
    for (GLint i = 0; i < n; ++i) if (!addCell()) return false;

    // initialize each robot's neighborhood
    if (!initNbrs()) return false;

    // organizes the cells into an initial formation (default line)
    Cell *c  = NULL;
    for (GLint i = 0; i < getNCells(); ++i)
    {
        if (!cells.getHead(c)) return false;
        c->x = f.getRadius() *
               ((GLfloat)i - (GLfloat)(getNCells() - 1) / 2.0f);
        c->y = 0.0f;
        c->setColor(DEFAULT_ROBOT_COLOR);
        c->setHeading(f.getHeading());
        ++cells;
    }
    return (getNCells() == n) &&
           sendMsg(new Formation(f), f.getSeedID(),
                   ID_OPERATOR,      CHANGE_FORMATION);
}   // initCells(const GLint, const Formation f)



//
// bool initNbrs(nNbrs)
// Last modified: 03Sep2006
//
// Initializes the neighborhood of each cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      nNbrs       in      the initial number of neighbors
//
bool Environment::initNbrs(const GLint nNbrs)
{
    Cell *c = NULL;
    for (GLint i = 0; i < getNCells(); ++i)
    {
        if (!cells.getHead(c))                            return false;
        c->clearNbrs();
        if ((i > 0)               && (!c->addNbr(i - 1))) return false;
        else c->leftNbr  = c->nbrWithID(i - 1);
        if ((i < getNCells() - 1) && (!c->addNbr(i + 1))) return false;
        else c->rightNbr = c->nbrWithID(i + 1);
        ++cells;
    }
    return true;
}   // initNbrs(const GLint)
