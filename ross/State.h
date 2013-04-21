//
// Filename:        "State.h"
//
// Programmer:      Ross Mead
// Last modified:   04Sep2006
//
// Description:     This structure defines a robot cell state.
//

// preprocessor directives
#ifndef STATE_H
#define STATE_H
#include "Formation.h"
using namespace std;

struct State
{

    // <data members>
	Formation                formation;     // the current formation
	Vector                   gradient;      // the formation gradient
	LinkedList<Relationship> rels;          // the formation relationships
    Vector                   transError;    // the summed translational error
    GLfloat                  rotError;      // the summed rotational error
	GLint                    step;          // the step in the formation



    // <constructors>

    //
    // State(f, grad, r, e, d, s)
    // Last modified: 04Sep2006
    //
    // Default constructor that initializes
    // this state to the parameterized values.
    //
    // Returns:     <none>
    // Parameters:
    //      f       in      the default formation
    //      grad    in      the default gradient
    //      r       in      the default list of relationships
    //      tError  in      the default translational error
    //      rError  in      the default rotational error
    //      s       in      the default step
    //
    State(const Formation                f      = Formation(),
          const Vector                   grad   = Vector(),
          const LinkedList<Relationship> r      = LinkedList<Relationship>(),
          const Vector                   tError = Vector(),
          const GLfloat                  rError = 0.0f,
          const GLint                    s      = 0)
          : formation(f),       gradient(grad),   rels(r),
            transError(tError), rotError(rError), step(s)
    {
    }   // State(const..{Formation, Vector, LL<Relationship>, Vector, GLint})
};  // State
#endif
