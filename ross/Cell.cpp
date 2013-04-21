//
// Filename:        "Cell.cpp"
//
// Programmer:      Ross Mead
// Last modified:   14May2007
//
// Description:     This class implements a robot cell.
//

// preprocessor directives
#include "Cell.h"
#include "Environment.h"



// <constructors>

//
// Cell(dx, dy, dz, theta, colorIndex)
// Last modified: 04Sep2006
//
// Default constructor that initializes
// this cell to the parameterized values.
//
// Returns:     <none>
// Parameters:
//      dx          in      the initial x-coordinate of the cell (default 0)
//      dy          in      the initial y-coordinate of the cell (default 0)
//      dz          in      the initial z-coordinate of the cell (default 0)
//      theta       in      the initial heading of the cell (default 0)
//      colorIndex  in      the initial array index of the color of the cell
//
Cell::Cell(const GLfloat dx,    const GLfloat dy, const GLfloat dz,
           const GLfloat theta, const Color   colorIndex)
    : State(), Neighborhood(), Robot()
{
    init(dx, dy, dz, theta, colorIndex);
}   // Cell(const GLfloat..<4>, const Color)



//
// Cell(c)
// Last modified: 27Aug2006
//
// Copy constructor that copies the contents of
// the parameterized cell into this cell.
//
// Returns:     <none>
// Parameters:
//      c       in/out      the cell being copied
//
Cell::Cell(const Cell &c): State(c), Neighborhood(c), Robot(c)
{
    leftNbr  = c.leftNbr;
    rightNbr = c.rightNbr;
}   // Cell(const Cell &)



// <destructors>

//
// ~Cell()
// Last modified: 27Aug2006
//
// Destructor that clears this cell.
//
// Returns:     <none>
// Parameters:  <none>
//
Cell::~Cell()
{
}   // ~Cell()



// <public mutator functions>

//
// bool setState(s)
// Last modified: 27Aug2006
//
// Attempts to set the state to the parameterized state,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      s       in/out  the state to be set to
//
bool Cell::setState(const State &s)
{
    *this = s;
    return true;
}   // setState(const State &)



//
// bool setNbrs(nh)
// Last modified: 27Aug2006
//
// Attempts to set the neighborhood to the parameterized neighborhood,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      nh      in/out  the neighborhood to be set to
//
bool Cell::setNbrs(Neighborhood &nh)
{
    *this = nh;
    return true;
}   // setNbrs(Neighborhood &)



//
// bool setRobot(r)
// Last modified: 14May2007
//
// Attempts to set the robot to the parameterized robot,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      r       in/out  the robot to be set to
//
bool Cell::setRobot(const Robot &r)
{
    *this = r;
    return true;
}   // setRobot(const Robot &)



// <public accessor functions>

//
// State getState() const
// Last modified: 27Aug2006
//
// Returns the state of this cell.
//
// Returns:     the state of this cell
// Parameters:  <none>
//
State Cell::getState() const
{
    return (State)*this;
}   // getState() const



//
// Neighborhood getNbrs() const
// Last modified: 27Aug2006
//
// Returns the neighborhood of this cell.
//
// Returns:     the neighborhood of this cell
// Parameters:  <none>
//
Neighborhood Cell::getNbrs() const
{
    return (Neighborhood)*this;
}   // getNbrs() const



//
// Robot getRobot() const
// Last modified: 27Aug2006
//
// Returns the robot of this cell.
//
// Returns:     the robot of this cell
// Parameters:  <none>
//
Robot Cell::getRobot() const
{
    return (Robot)*this;
}   // getRobot() const



// <virtual public utility functions>

//
// void draw()
// Last modified: 27Aug2006
//
// Renders the cell as a robot.
//
// Returns:     <none>
// Parameters:  <none>
//
void Cell::draw()
{
    Robot::draw();
}   // draw()



//
// void step()
// Last modified: 27Aug2006
//
// Processes packets received and updates the state of the cell,
// which is then broadcast within the neighborhood of the cell.
//
// Returns:     <none>
// Parameters:  <none>
//
void Cell::step()
{
    if (processPackets())
    {
        updateState();
        sendStateToNbrs();
    }
    Robot::step();
}   // step()



//
// void updateState()
// Last modified: 27Aug2006
//
// Updates the state of the cell based upon the
// current states of the neighbors of the cell.
//
// Returns:     <none>
// Parameters:  <none>
//
void Cell::updateState()
{
    Neighbor currNbr;
    for (GLint i = 0; i < getNNbrs(); ++i)
    {
        if (!getHead(currNbr)) break;

        // change formation if a neighbor has changed formation
        if (getNbr(0)->formation.getFormationID() > formation.getFormationID())
            changeFormation(getNbr(0)->formation, *getNbr(0));
        getNbr(0)->relActual = getRelationship(currNbr.ID);
        ++(*this);
    }
    rels = getRelationships();
	if(rels.getSize())
	{
		// reference the neighbor with the smallest gradient
		// to establish correct position in formation
		Neighbor     *refNbr = nbrWithMinGradient();
		Relationship *nbrRel = relWithID(refNbr->rels, ID);
		if ((formation.getSeedID() != ID) && (refNbr != NULL) && (nbrRel != NULL))
		{

			// error (state) is based upon the accumulated error in the formation
			nbrRel->relDesired.rotateRelative(-refNbr->rotError);
			GLfloat theta = scaleDegrees(nbrRel->relActual.angle() -
										 (-refNbr->relActual).angle());
			rotError      = scaleDegrees(theta + refNbr->rotError);
			transError    = nbrRel->relDesired - nbrRel->relActual +
							refNbr->transError;
			transError.rotateRelative(-theta);
			if (transError.norm() > threshold()) moveArc(transError);
			else
				if (abs(rotError) > angThreshold())
					moveArc(0.0, degreesToRadians(-rotError));
				/*if (abs(scaleDegrees(refNbr->relActual.angle() -
									 refNbr->relDesired.angle())) > angThreshold())
					orientTo(refNbr->relActual, refNbr->relDesired.angle());*/
				else moveStop();
		}
		else moveStop();
	}
}   // updateState()



// <virtual public neighborhood functions>

//
// bool changeFormation()
// Last modified: 27Aug2006
//
// Attempts to change the formation of the cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      f       in/out      the formation to change to
//      n       in          the neighbor instigating the formation change
//
bool Cell::changeFormation(const Formation &f, Neighbor n)
{
    formation = f;
    if (formation.getSeedID() == ID)
    {
        gradient   = formation.getSeedGradient();
        transError = Vector();
        rotError   = 0.0f;
    }
    else
    {
        Relationship *nbrRel = relWithID(n.rels, ID);
        if (nbrRel == NULL) return false;
        nbrRel->relDesired.rotateRelative(n.formation.getHeading());
        gradient             = n.gradient + nbrRel->relDesired;
        transError           = Vector();
        rotError             = 0.0f;
    }
    LinkedList<Vector> r = formation.getRelationships(gradient);
    if (leftNbr  != NULL) leftNbr->relDesired  = r[LEFT_NBR_INDEX];
    if (rightNbr != NULL) rightNbr->relDesired = r[RIGHT_NBR_INDEX];
    return true;
}   // changeFormation(const Formation &, Neighbor)



//
// bool sendStateToNbrs()
// Last modified: 27Aug2006
//
// Attempts to broadcast the state of the cell
// to the neighborhood of the cell, returning
// true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
bool Cell::sendStateToNbrs()
{
    Neighbor curr;
    for (GLint i = 0; i < getNNbrs(); ++i)
        if (!sendState(getNbr(i)->ID)) return false;
    return true;
}   // sendStateToNbrs()



//
// bool sendState(toID)
// Last modified: 27Aug2006
//
// Attempts to send the state of the cell
// to the neighbor with the parameterized ID,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      toID    in      the ID of the receiving neighbor
//
bool Cell::sendState(const GLint toID)
{
    return sendMsg(new State(*this), toID, STATE);
}   // sendState(const GLint)



//
// bool processPackets()
// Last modified: 27Aug2006
//
// Attempts to process all packets received by the cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
bool Cell::processPackets()
{
    Packet p;
    while (msgQueue.dequeue(p))
        if (!processPacket(p)) return false;
    return true;
}   // processPackets()



//
// bool processPacket(p)
// Last modified: 22Dec2006
//
// Attempts to process the parameterized packet,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      p       in/out      the packet to be processed
//
bool Cell::processPacket(Packet &p)
{
    bool success = false;
    if ((p.fromOperator()) && (p.type == CHANGE_FORMATION))
        success  = changeFormation(*((Formation *)p.msg));
    else if ((isNbr(p.fromID)) || (p.fromBroadcast()))
        switch(p.type)
        {
            case STATE:
                success = (p.msg == NULL) ?
                    false : updateNbr(p.fromID, *((State *)p.msg));
                delete (State *)p.msg;
                p.msg   = NULL;
                break;
            default: break;
        }
    return success;
}   // processPacket(Packet &)



// <virtual overloaded operators>

//
// Cell& =(s)
// Last modified: 27Aug2006
//
// Copies the contents of the parameterized state into this cell.
//
// Returns:     this cell
// Parameters:
//      s       in/out      the state being copied
//
Cell& Cell::operator =(const State &s)
{
    return *this = s;
}   // =(const State &)



//
// Cell& =(nh)
// Last modified: 27Aug2006
//
// Copies the contents of the parameterized neighborhood into this cell.
//
// Returns:     this cell
// Parameters:
//      nh      in/out      the neighborhood being copied
//
Cell& Cell::operator =(const Neighborhood &nh)
{
    return *this = nh;
}   // =(const Neighborhood &)



//
// Cell& =(r)
// Last modified: 27Aug2006
//
// Copies the contents of the parameterized robot into this cell.
//
// Returns:     this cell
// Parameters:
//      r       in/out      the robot being copied
//
Cell& Cell::operator =(const Robot &r)
{
    return *this = r;
}   // =(const Robot &)



// <virtual protected utility functions>

//
// bool init(dx, dy, dz, theta, colorIndex)
// Last modified: 27Aug2006
//
// Initializes the cell to the parameterized values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      dx          in      the initial x-coordinate of the cell (default 0)
//      dy          in      the initial y-coordinate of the cell (default 0)
//      dz          in      the initial z-coordinate of the cell (default 0)
//      theta       in      the initial heading of the cell (default 0)
//      colorIndex  in      the initial array index of the color of the cell
//
bool Cell::init(const GLfloat dx,    const GLfloat dy, const GLfloat dz,
                const GLfloat theta, const Color   colorIndex)
{
    leftNbr = rightNbr = NULL;
    return true;
}   // init(const GLfloat..<4>, const Color)
