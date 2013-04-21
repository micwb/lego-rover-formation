//
// Filename:        "Environment.h"
//
// Programmer:      Ross Mead
// Last modified:   07Sep2006
//
// Description:     This class describes a robot cell environment.
//

// preprocessor directives
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "Cell.h"
using namespace std;

// global constants
static const Color DEFAULT_ENV_COLOR = BLACK;

class Environment
{
    public:

        // <public data members>
        GLfloat color[3];

        // <constructors>
		Environment() {};
        //Environment(const GLint     n          = 0,
        //            const Formation f          = Formation(),
        //            const Color     colorIndex = DEFAULT_ENV_COLOR);
        Environment(const Environment &e);

        // <destructors>
        virtual ~Environment();

        // <virtual public mutator functions>
        virtual bool setColor(const GLfloat r,
                              const GLfloat g,
                              const GLfloat b);
        virtual bool setColor(const GLfloat clr[3]);
        virtual bool setColor(const Color colorIndex = DEFAULT_VECTOR_COLOR);

        // <public mutator functions>
        bool addCell(Cell *c = NULL);
		bool addCell(int xi, int yi, float heading, Cell *c = NULL);
        bool removeCell();
        bool removeCell(Cell* &c);

        // <public accessor functions>
        Cell*              getCell(GLint pos) const;
        LinkedList<Cell *> getCells();
        GLint              getNCells() const;

        // <virtual public utility functions>
        virtual void draw();
        virtual bool step();
        virtual void clear();

        // <public utility functions>
        Vector  getRelationship(const GLint toID, const GLint fromID);
        GLfloat getDistanceTo(const GLint id)   const;
        GLfloat getAngleTo(const GLint id)      const;
        bool    sendMsg(const Message &msg    = NULL,
                        const GLint    toID   = ID_BROADCAST,
                        const GLint    fromID = ID_OPERATOR,
                        const GLint    type   = HEARTBEAT);
        bool    sendPacket(const Packet &p = Packet());
        bool    forwardPacket(const Packet &p);
        bool    forwardPackets();

        // <public utility cell functions>
        bool    showLine(const bool show);
        bool    showHead(const bool show);
        bool    showPos(const bool show);
        bool    showHeading(const bool show);

        // <public wrapper of some protected functions>
        virtual bool initRobots(const Formation f);
        virtual bool areRobotsReady();

    protected:

        // <protected data members>
        LinkedList<Cell *> cells;
        Queue<Packet>      msgQueue;

        // <virtual protected utility functions>
        virtual bool init(const GLint     n          = 0,
                          const Formation f          = Formation(),
                          const Color     colorIndex = DEFAULT_ENV_COLOR);
        virtual bool initCells(const GLint     n = 0,
                               const Formation f = Formation());
        virtual bool initNbrs(const GLint nNbrs = 0);
};  // Environment
#endif
