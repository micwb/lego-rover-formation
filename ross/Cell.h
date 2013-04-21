//
// Filename:        "Cell.h"
//
// Programmer:      Ross Mead
// Last modified:   27Aug2006
//
// Description:     This class describes a robot cell.
//

// preprocessor directives
#ifndef CELL_H
#define CELL_H
#include "Neighborhood.h"
#include "Robot.h"
using namespace std;

// message type index values
enum MessageType {HEARTBEAT, STATE, CHANGE_FORMATION};

// global constants
static const Color DEFAULT_CELL_COLOR = DEFAULT_ROBOT_COLOR;
static const GLint LEFT_NBR_INDEX     = 0;
static const GLint RIGHT_NBR_INDEX    = 1;

class Cell: protected State, public Neighborhood, public Robot
{
    friend class Environment;

    public:

        // <constructors>
        Cell(const GLfloat dx         = 0.0f,
             const GLfloat dy         = 0.0f,
             const GLfloat dz         = 0.0f,
             const GLfloat theta      = 0.0f,
             const Color   colorIndex = DEFAULT_CELL_COLOR);
        Cell(const Cell &r);

        // <destructors>
        virtual ~Cell();

        // <public mutator functions>
        bool setState(const State &s);
        bool setNbrs(Neighborhood &nh);
        bool setRobot(const Robot &r);

        // <public accessor functions>
        State        getState() const;
        Neighborhood getNbrs()  const;
        Robot        getRobot() const;

        // <virtual public utility functions>
        virtual void draw();
        virtual void step();
        virtual void updateState();

        // <virtual public neighborhood functions>
        virtual bool changeFormation(const Formation &f,
                                     Neighbor         n = Neighbor());
        virtual bool sendStateToNbrs();
        virtual bool sendState(const GLint);
        virtual bool processPackets();
        virtual bool processPacket(Packet &p);

        // <virtual overloaded operators>
        virtual Cell& operator =(const State &s);
        virtual Cell& operator =(const Neighborhood &nh);
        virtual Cell& operator =(const Robot &r);

    protected:

        // <protected data members>
        Neighbor *leftNbr, *rightNbr;

        // <virtual protected utility functions>
        virtual bool init(const GLfloat dx         = 0.0f,
                          const GLfloat dy         = 0.0f,
                          const GLfloat dz         = 0.0f,
                          const GLfloat theta      = 0.0f,
                          const Color   colorIndex = DEFAULT_CELL_COLOR);
};  // Cell
#endif
