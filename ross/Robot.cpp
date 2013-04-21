//
// Filename:        "Robot.cpp"
//
// Programmer:      Ross Mead
// Last modified:   17May2007
//
// Description:     This class implements a 2-dimensional robot.
//

// preprocessor directives
#include "Environment.h"
#include "Robot.h"
#include "../formationcontrol/helpers.h"
#include <QtCore/QString>
#include "simulator.h"
#include "../formationcontrol/types.h"

const float MAX_ROBOT_SPEED_DPS        = 360.0;
const float MAX_ROBOT_SPEED_MPS        = 130.375985 / 1000;
const float MAX_ROBOT_SPEED_SPS        = 1000.0;

// <constructors>
GLint Robot::nRobots = 0;   // initializes the number of robots to 0

//
// Robot(dx, dy, dz, theta, colorIndex)
// Last modified: 04Sep2006
//
// Default constructor that initializes
// this robot to the parameterized values.
//
// Returns:     <none>
// Parameters:
//      dx          in      the initial x-coordinate of the robot (default 0)
//      dy          in      the initial y-coordinate of the robot (default 0)
//      dz          in      the initial z-coordinate of the robot (default 0)
//      theta       in      the initial heading of the robot (default 0)
//      colorIndex  in      the initial array index of the color of the robot
//
Robot::Robot(const GLfloat dx,    const GLfloat dy, const GLfloat dz,
             const GLfloat theta, const Color   colorIndex)
{
	linearSpeedSteps = 0;
	angularSpeedSteps = 0;
    init(dx, dy, dz, theta, colorIndex);
    ID = nRobots++;
}   // Robot(const GLfloat..<4>, const Color)



//
// Robot(r)
// Last modified: 03Sep2006
//
// Copy constructor that copies the contents of
// the parameterized robot into this robot.
//
// Returns:     <none>
// Parameters:
//      r       in/out      the robot being copied
//
Robot::Robot(const Robot &r)
{
    init(r.x, r.y, r.z, r.getHeading(), DEFAULT_ROBOT_COLOR);
    setColor(r.color);
    for (GLint i = 0; i < 3; ++i)
    {
        translate[i] = r.translate[i];
        rotate[i]    = r.rotate[i];
        scale[i]     = r.scale[i];
    }
    showLine    = r.showLine;
    showHead    = r.showHead;
    showPos     = r.showPos;
    showHeading = r.showHeading;
    ID          = r.ID;
    env         = r.env;
    msgQueue    = r.msgQueue;
	linearSpeedSteps = 0;
	angularSpeedSteps = 0;
}   // Robot(const Robot &)



// <destructors>

//
// ~Robot()
// Last modified: 03Sep2006
//
// Destructor that clears this robot.
//
// Returns:     <none>
// Parameters:  <none>
//
Robot::~Robot()
{
}   // ~Robot()



// <virtual public mutator functions>

//
// bool setRadius(s)
// Last modified: 03Sep2006
//
// Attempts to set the radius to the parameterized radius,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      r       in      the radius to be set to
//
bool Robot::setRadius(const GLfloat r)
{
    if (!Circle::setRadius(r)) return false;
    return behavior.setMaxSpeed(maxSpeed());
}   // setRadius(const GLfloat)



//
// bool setHeading(theta)
// Last modified: 03Sep2006
//
// Attempts to set the heading to the parameterized heading,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      theta   in      the heading to be set to
//
bool Robot::setHeading(const GLfloat theta)
{
    return heading.setPolar(radius + VECTOR_HEAD_HEIGHT, theta);
}   // setHeading(const GLfloat)



//
// bool setEnvironment(e)
// Last modified: 03Sep2006
//
// Attempts to set the environment to the parameterized environment,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      e       in      the envirnment to be set to
//
bool Robot::setEnvironment(Environment *e)
{
    env = e;
    return true;
}   // setEnvironment(Environment *)



//
// void translateRelative(v)
// Last modified: 03Sep2006
//
// Translates the robot relative to itself based
// on the parameterized translation vector.
//
// Returns:     <none>
// Parameters:
//      v       in      the translation vector
//
void Robot::translateRelative(Vector v)
{
    v.rotateRelative(getHeading());
    x += v.x;
    y += v.y;
}   // translateRelative(const Vector)



//
// void translateRelative(dx, dy)
// Last modified: 03Sep2006
//
// Translates the robot relative to itself based
// on the parameterized x-/y-coordinate translations.
//
// Returns:     <none>
// Parameters:
//      dx      in      the x-coordinate translation
//      dy      in      the y-coordinate translation
//
void Robot::translateRelative(const GLfloat dx, const GLfloat dy)
{
    translateRelative(Vector(dx, dy));
	int xi;
    f2idx(dx, &xi, windowSize[0], windowSize[1]);
    float dist_m = ((float)xi) / ((float)gCameraScalePPM); //dist_mm / 1000;
    //float elapsed = 0;
    //if(pElapsedTimer)
    //    elapsed = pElapsedTimer->elapsed();
    float mps = dist_m / (STI_SEC + 0);
    linearSpeedSteps = MPS_TO_SPS(mps);
    //if(terminalList.count() && ID == 0)
    //    terminalList.at(ID).pSerPort->write(QByteArray((QString("D,") +
    //                                                   QString::number(linearSpeedSteps-angularSpeedSteps) +
    //                                                   QString(",") +
    //                                                   QString::number(linearSpeedSteps+angularSpeedSteps) +
    //                                                   QString("\r")).toAscii()));
    //linearSpeedSteps = 0;
}   // translateRelative(const GLfloat, const GLfloat)



//
// void rotateRelative(theta)
// Last modified: 03Sep2006
//
// Rotates the robot about itself (in 2-dimensions)
// based on the parameterized rotation angle.
//
// Returns:     <none>
// Parameters:
//      theta   in      the rotation angle
//
void Robot::rotateRelative(GLfloat theta)
{
    heading.rotateRelative(theta);
    float elapsed = 0;
    //if(pElapsedTimer)
        //elapsed = pElapsedTimer->elapsed();
    float dps = theta / (STI_SEC - 0);
    angularSpeedSteps = (DPS_TO_SPS(dps))*(1.2);
    if(terminalList.count()/* && ID == 0*/)
        terminalList.at(ID).pSerPort->write(QByteArray((QString("D,") +
                                                        QString::number(linearSpeedSteps-angularSpeedSteps) +
                                                        QString(",") +
                                                        QString::number(linearSpeedSteps+angularSpeedSteps) +
														QString("\r")).toStdString().c_str()));
	//angularSpeedSteps = 0;
}   // rotateRelative(GLfloat)



// <virtual public accessor functions>

//
// Environment* getEnvironment() const
// Last modified: 03Sep2006
//
// Returns the environment of this robot.
//
// Returns:     the environment of this robot
// Parameters:  <none>
//
Environment* Robot::getEnvironment() const
{
    return env;
}   // getEnvironment() const



// <public accessor functions>

//
// GLint getID() const
// Last modified: 03Sep2006
//
// Returns the ID of this robot.
//
// Returns:     the ID of this robot
// Parameters:  <none>
//
GLint Robot::getID() const
{
    return ID;
}   // getID()



//
// GLfloat getHeading() const
// Last modified: 03Sep2006
//
// Returns the heading of this robot.
//
// Returns:     the heading of this robot
// Parameters:  <none>
//
GLfloat Robot::getHeading() const
{
    return heading.angle();
}   // getHeading() const



//
// GLfloat getTransVel() const
// Last modified: 03Sep2006
//
// Returns the translational velocity of this robot.
//
// Returns:     the translational velocity of this robot
// Parameters:  <none>
//
GLfloat Robot::getTransVel() const
{
    return behavior.getTransVel();
}   // getTransVel() const



//
// GLfloat getRotVel() const
// Last modified: 03Sep2006
//
// Returns the rotational velocity of this robot.
//
// Returns:     the rotational velocity of this robot
// Parameters:  <none>
//
GLfloat Robot::getRotVel() const
{
    return behavior.getRotVel();
}   // getRotVel() const



//
// GLfloat getAngVel() const
// Last modified: 03Sep2006
//
// Returns the angular velocity of this robot.
//
// Returns:     the angular velocity of this robot
// Parameters:  <none>
//
GLfloat Robot::getAngVel() const
{
    return radiansToDegrees(getRotVel() / radius);
}   // getAngVel() const



//
// GLfloat getVelocity() const
// Last modified: 03Sep2006
//
// Returns the velocity of this robot.
//
// Returns:     the velocity of this robot
// Parameters:  <none>
//
GLfloat Robot::getVelocity() const
{
    return behavior.getVelocity();
}   // getVelocity() const



//
// GLfloat getArcRadius() const
// Last modified: 03Sep2006
//
// Returns the arc radius based on the translational
// and rotational velocities this robot.
//
// Returns:     the arc radius of this robot
// Parameters:  <none>
//
GLfloat Robot::getArcRadius() const
{
    return (behavior.getRotVel() == 0.0f) ? 0.0f : 
            radius * behavior.getTransVel() / behavior.getRotVel();
}   // getArcRadius() const



// <virtual public utility functions>

//
// void draw()
// Last modified: 27Aug2006
//
// Renders the robot as a circle with a vector heading.
//
// Returns:     <none>
// Parameters:  <none>
//
void Robot::draw()
{
    if ((color[GLUT_RED]   == COLOR[INVISIBLE][GLUT_RED])   &&
        (color[GLUT_GREEN] == COLOR[INVISIBLE][GLUT_GREEN]) &&
        (color[GLUT_BLUE]  == COLOR[INVISIBLE][GLUT_BLUE])) return;

    // draw a circle representing the robot
    Circle::draw();

    // draw a vector representing the robot heading
    if (showHeading)
    {
        glPushMatrix();
            glRotated(rotate[0], 0, 0, 1);
            glRotated(rotate[1], 0, 0, 1);
            glRotated(rotate[2], 0, 0, 1);
            heading.translated(x + translate[0],
                               y + translate[1],
                               z + translate[2]);
            heading.scaled(radius / DEFAULT_ROBOT_RADIUS);
            heading.setColor(color);
            heading.draw();
        glPopMatrix();
    }
}   // draw()



//
// void step()
// Last modified: 03Sep2006
//
// Executes the appropriate active behavior.
//
// Returns:     <none>
// Parameters:  <none>
//
void Robot::step()
{
	if(terminalList.count()/* && ID == 0*/)
        terminalList.at(ID).pSerPort->write(QByteArray((QString("D,") +
                                                        QString::number(0) +
                                                        QString(",") +
                                                        QString::number(0) +
														QString("\r")).toStdString().c_str()));
    if (behavior.isActive())
	{
        translateRelative(getTransVel());
        rotateRelative(getAngVel());
    }
}   // step()



// <public utility functions>

//
// Vector getRelationship(target)
// Last modified: 03Sep2006
//
// Returns the relationship from this robot
// to the parameterized target vector.
//
// Returns:     the relationship from this robot to the target vector
// Parameters:
//      target  in/out  the target vector being related to
//
Vector Robot::getRelationship(Vector &target) const
{
    Vector temp = target - *this;
    temp.rotateRelative(-getHeading());
    return temp;
}   // getRelationship(Vector &) const



//
// GLfloat getDistanceTo(target)
// Last modified: 06Mar2007
//
// Returns the distance from this robot
// to the parameterized target vector.
//
// Returns:     the distance from this robot to the target vector
// Parameters:
//      target  in/out  the target vector being related to
//
GLfloat Robot::getDistanceTo(Vector &target) const
{
    return getRelationship(target).norm();
}   // getDistanceTo(Vector &) const



//
// GLfloat getAngleTo(target)
// Last modified: 06Mar2007
//
// Returns the angle from this robot
// to the parameterized target vector.
//
// Returns:     the angle from this robot to the target vector
// Parameters:
//      target  in/out  the target vector being related to
//
GLfloat Robot::getAngleTo(Vector &target) const
{
    return getRelationship(target).angle();
}   // getAngleTo(Vector &) const



//
// GLfloat maxSpeed() const
// Last modified: 03Sep2006
//
// Returns the max speed of this robot.
//
// Returns:     the max speed of this robot
// Parameters:  <none>
//
GLfloat Robot::maxSpeed() const
{
    return FACTOR_MAX_SPEED * radius;
}   // maxSpeed() const



//
// GLfloat maxAngSpeed() const
// Last modified: 03Sep2006
//
// Returns the max angular speed of this robot.
//
// Returns:     the max angular speed of this robot
// Parameters:  <none>
//
GLfloat Robot::maxAngSpeed() const
{
    return radiansToDegrees(maxSpeed() / radius);
}   // maxAngSpeed() const



//
// GLfloat threshold() const
// Last modified: 03Sep2006
//
// Returns the minimum movement threshold of this robot.
//
// Returns:     the minimum movement threshold of this robot
// Parameters:  <none>
//
GLfloat Robot::threshold() const
{
    return FACTOR_THRESHOLD * maxSpeed();
}   // threshold() const



//
// GLfloat angThreshold() const
// Last modified: 03Sep2006
//
// Returns the minimum angular movement threshold of this robot.
//
// Returns:     the minimum angular movement threshold of this robot
// Parameters:  <none>
//
GLfloat Robot::angThreshold() const
{
    return FACTOR_THRESHOLD * maxAngSpeed();
}   // angThreshold() const



//
// GLfloat collisionRadius() const
// Last modified: 15Nov2006
//
// Returns the minimum collision radius of this robot.
//
// Returns:     the minimum collision radius of this robot
// Parameters:  <none>
//
GLfloat Robot::collisionRadius() const
{
    return FACTOR_COLLISION_RADIUS * radius;
}   // collisionRadius() const



// <virtual public environment functions>

//
// Vector getRelationship(toID)
// Last modified: 03Sep2006
//
// Returns the relationship from this robot
// to the robot with the parameterized ID.
//
// Returns:     the relationship from this robot to another robot
// Parameters:
//      toID    in      the ID of the robot being related to
//
Vector Robot::getRelationship(const GLint toID) const
{
    return (env == NULL) ? Vector() : env->getRelationship(toID, ID);
}   // getRelationship(const GLint) const



//
// GLfloat getDistance(toID)
// Last modified: 03Sep2006
//
// Returns the distance from this robot
// to the robot with the parameterized ID.
//
// Returns:     the distance from this robot to another robot
// Parameters:
//      toID    in      the ID of the robot being related to
//
GLfloat Robot::getDistanceTo(const GLint toID) const
{
    return getRelationship(toID).norm();
}   // getDistanceTo(const GLint) const



//
// GLfloat getAngle(toID)
// Last modified: 03Sep2006
//
// Returns the angle from this robot
// to the robot with the parameterized ID.
//
// Returns:     the angle from this robot to another robot
// Parameters:
//      toID    in      the ID of the robot being related to
//
GLfloat Robot::getAngleTo(const GLint toID) const
{
    return getRelationship(toID).angle();
}   // getAngleTo(const GLint) const



// <virtual public environment functions>

//
// bool sendMsg(msg, toID, type)
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
//      type    in      the type of message being sent
//
bool Robot::sendMsg(const Message &msg, const GLint toID, const GLint type)
{
    if (sendPacket(Packet(msg, toID, ID, type))) return true;
    delete msg;
    return false;
}   // sendMsg(const Message &, const GLint, const GLint)



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
bool Robot::sendPacket(const Packet &p)
{
    if ((env != NULL) && (env->sendPacket(p))) return true;
    delete p.msg;
    return false;
}   // sendPacket(const Packet &)



// <public primitive behaviors>

//
// Behavior moveArc(target)
// Last modified: 03Sep2006
//
// Moves the robot using the parameterized movement vector,
// activating and returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target move of the behavior
//
Behavior Robot::moveArc(const Vector &target)
{
    return behavior = moveArcBehavior(target);
}   // moveArc(const Vector &)



//
// Behavior moveArcBehavior(target)
// Last modified: 17May2007
//
// Moves the robot using the parameterized movement vector,
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target move of the behavior
//
Behavior Robot::moveArcBehavior(const Vector &target)
{
    GLfloat r     = target.norm();
    if (r <= threshold()) return moveStop();
    GLfloat theta = degreesToRadians(target.angle());
    if (theta == 0.0f)    return moveForwardBehavior(r);
    else return moveArcBehavior((abs(theta) >
                                degreesToRadians(angThreshold())) ?
                                0.0f :
                                r * theta / sin(theta), getDiameter() * theta);
}   // moveArcBehavior(const Vector &)



//
// Behavior moveArc(t, r, s)
// Last modified: 03Sep2006
//
// Moves the robot using the parameterized translational
// and rotational velocities, activating and returning
// the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      t       in      the translational velocity of the behavior
//      r       in      the rotational velocity of the behavior
//      s       in      the status of the behavior (default ACTIVE)
//
Behavior Robot::moveArc(const GLfloat t, const GLfloat r, const Status s)
{
    return behavior = moveArcBehavior(t, r, s);
}   // moveArc(const GLfloat, const GLfloat, const Status)



//
// Behavior moveArcBehavior(t, r, s)
// Last modified: 03Sep2006
//
// Moves the robot using the parameterized translational
// and rotational velocities, returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      t       in      the translational velocity of the behavior
//      r       in      the rotational velocity of the behavior
//      s       in      the status of the behavior (default ACTIVE)
//
Behavior Robot::moveArcBehavior(const GLfloat t,
                                const GLfloat r,
                                const Status  s)
{
    return Behavior(s, t, r, maxSpeed());
}   // moveArcBehavior(const GLfloat, const GLfloat, const Status)



//
// Behavior moveForward(vel)
// Last modified: 03Sep2006
//
// Moves the robot forward using the parameterized robot velocity,
// activating and returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      vel     in      the velocity of the behavior
//
Behavior Robot::moveForward(const GLfloat speed)
{
    return behavior = moveForwardBehavior(speed);
}   // moveForward(const GLfloat)



//
// Behavior moveForwardBehavior(vel)
// Last modified: 03Sep2006
//
// Moves the robot forward using the parameterized robot velocity,
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      vel     in      the velocity of the behavior
//
Behavior Robot::moveForwardBehavior(const GLfloat vel)
{
    return moveArcBehavior(vel, 0.0f);
}   // moveForwardBehavior(const GLfloat)



//
// Behavior moveBackward(vel)
// Last modified: 03Sep2006
//
// Moves the robot backward using the parameterized robot velocity,
// activating and returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      vel     in      the velocity of the behavior
//
Behavior Robot::moveBackward(const GLfloat vel)
{
    return behavior = moveForwardBehavior(vel);
}   // moveForward(const GLfloat)



//
// Behavior moveBackwardBehavior(vel)
// Last modified: 03Sep2006
//
// Moves the robot backward using the parameterized robot velocity,
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      vel     in      the velocity of the behavior
//
Behavior Robot::moveBackwardBehavior(const GLfloat vel)
{
    return moveArcBehavior(-vel, 0.0f);
}   // moveForwardBehavior(const GLfloat)



//
// Behavior moveStop()
// Last modified: 03Sep2006
//
// Stops the robot from moving, activating and
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:  <none>
//
Behavior Robot::moveStop()
{
    return behavior = moveStopBehavior();
}   // moveStop()



//
// Behavior moveStopBehavior()
// Last modified: 03Sep2006
//
// Stops the robot from moving,
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:  <none>
//
Behavior Robot::moveStopBehavior()
{
    return moveArcBehavior(0.0f, 0.0f, DONE);
}   // moveStopBehavior()



// <public pair behaviors>

//
// Behavior orientTo(target, theta)
// Last modified: 03Sep2006
//
// Rotates the robot to the parameterized heading
// relative to the paratermized target, activating and
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target to orient to
//      theta   in      the heading to maintain to the target
//
Behavior Robot::orientTo(const Vector &target, const GLfloat theta)
{
    return behavior = orientToBehavior(target, theta);
}   // orientTo(const Vector &, const GLfloat)



//
// Behavior orientToBehavior(target, theta)
// Last modified: 03Sep2006
//
// Rotates the robot to the parameterized heading
// relative to the paratermized target,
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target to orient to
//      theta   in      the heading to maintain to the target
//
Behavior Robot::orientToBehavior(const Vector &target, const GLfloat theta)
{
    GLfloat delta   = scaleDegrees(target.angle() - theta);
    if (abs(delta) <= angThreshold()) return moveStopBehavior();
    return moveArcBehavior(0.0f, degreesToRadians(delta));
}   // orientToBehavior(const Vector &, const GLfloat)



//
// Behavior follow(target, dist)
// Last modified: 03Sep2006
//
// Directs the robot to follow the parameterized target
// maintaining the parameterized distance, activating and
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target to follow
//      dist    in      the distance to maintain from the target
//
Behavior Robot::follow(const Vector &target, const GLfloat dist)
{
    return behavior = followBehavior(target, dist);
}   // follow(const Vector &, const GLfloat)



//
// Behavior followBehavior(target, dist)
// Last modified: 03Sep2006
//
// Directs the robot to follow the parameterized target
// maintaining the parameterized distance,
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target to follow
//      dist    in      the distance to maintain from the target
//
Behavior Robot::followBehavior(const Vector &target, const GLfloat dist)
{
    GLfloat  r   = target.norm();
    if (r <= threshold()) return moveStopBehavior();
    Behavior beh = orientToBehavior(target, 0.0f);
    if ((beh.isDone()) && (r > dist)) return moveForwardBehavior(r - dist);
    return beh;
}   // followBehavior(const Vector &, const GLfloat)



//
// Behavior avoid(target, dist)
// Last modified: 03Sep2006
//
// Directs the robot to avoid the parameterized target,
// maintaining the parameterized distance, activating and
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target to avoid
//      dist    in      the distance to maintain from the target
//
Behavior Robot::avoid(const Vector &target, const GLfloat dist)
{
    return behavior = avoidBehavior(target, dist);
}   // avoid(const Vector &, const GLfloat)



//
// Behavior avoidBehavior(target, dist)
// Last modified: 03Sep2006
//
// Directs the robot to avoid the parameterized target,
// maintaining the parameterized distance,
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target to avoid
//      dist    in      the distance to maintain from the target
//
Behavior Robot::avoidBehavior(const Vector &target, const GLfloat dist)
{
    GLfloat r                     = target.norm();
    if (r < dist) return sumBehaviors(orientToBehavior(target, 180.0f),
                                      moveForwardBehavior(dist - r));
    return moveStopBehavior();
}   // avoidBehavior(const Vector &, const GLfloat)



//
// Behavior orientForOrbit(target, dist)
// Last modified: 03Sep2006
//
// Orients the robot with respect to the parameterized target
// such that if the robot were moving forward, it would move
// in a circular path maintaining the parameterized distance
// around the target, activating and returning the appropriate
// robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target to orient to for orbit
//      dist    in      the distance to maintain from the target
//
Behavior Robot::orientForOrbit(const Vector &target, const GLfloat dist)
{
    return behavior = orientForOrbitBehavior(target, dist);
}   // orientForOrbit(const Vector &, const GLfloat)



//
// Behavior orientForOrbitBehavior(target, dist)
// Last modified: 03Sep2006
//
// Orients the robot with respect to the parameterized target
// such that if the robot were moving forward, it would move
// in a circular path maintaining the parameterized distance
// around the target, returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target to orient to for orbit
//      dist    in      the distance to maintain from the target
//
Behavior Robot::orientForOrbitBehavior(const Vector &target,
                                       const GLfloat dist)
{
    GLfloat  r = target.norm(),            dir = 0.0f;
    if      (r > dist + collisionRadius()) dir = 0.0f;
    else if (r < dist - collisionRadius()) dir = 180.0f;
    else dir = 180.0f - r * 90.0f / collisionRadius();
    return orientToBehavior(target, dir);
}   // orientForOrbitBehavior(const Vector &, const GLfloat)



//
// Behavior orbit(target, dist)
// Last modified: 03Sep2006
//
// Guides the robot around the parameterized target
// in a circular path maintaining the parameterized distance,
// activating and returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target to orbit
//      dist    in      the distance to maintain from the target
//
Behavior Robot::orbit(const Vector &target, const GLfloat dist)
{
    return behavior = orbitBehavior(target, dist);
}   // orbit(const Vector &, const GLfloat)



//
// Behavior orbitBehavior(target, dist)
// Last modified: 03Sep2006
//
// Guides the robot around the parameterized target
// in a circular path maintaining the parameterized distance,
// returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      target  in/out  the target to orbit
//      dist    in      the distance to maintain from the target
//
Behavior Robot::orbitBehavior(const Vector &target, const GLfloat dist)
{
    return sumBehaviors(orientForOrbitBehavior(target, dist),
                        moveForwardBehavior(maxSpeed()));
}   // orbit(const Vector &, const GLfloat)



// <virtual protected utility functions>

//
// bool init(dx, dy, dz, theta, colorIndex)
// Last modified: 07Sep2006
//
// Initializes the robot to the parameterized values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      dx          in      the initial x-coordinate of the robot (default 0)
//      dy          in      the initial y-coordinate of the robot (default 0)
//      dz          in      the initial z-coordinate of the robot (default 0)
//      theta       in      the initial heading of the robot (default 0)
//      colorIndex  in      the initial array index of the color of the robot
//
bool Robot::init(const GLfloat dx,    const GLfloat dy, const GLfloat dz,
                 const GLfloat theta, const Color   colorIndex)
{
    Circle::init(dx, dy, dz, DEFAULT_ROBOT_RADIUS, colorIndex);
    setHeading(theta);
    behavior         = DEFAULT_ROBOT_BEHAVIOR;
    behavior.setMaxSpeed(maxSpeed());
    showHeading      = DEFAULT_ROBOT_SHOW_HEADING;
    heading.showLine = DEFAULT_ROBOT_SHOW_LINE;
    heading.showHead = DEFAULT_ROBOT_SHOW_HEAD;
    setEnvironment(NULL);
    return true;
}   // init(const GLfloat..<4>, const Color)
