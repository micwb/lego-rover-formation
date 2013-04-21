//
// Filename:        "Neighborhood.cpp"
//
// Programmer:      Ross Mead
// Last modified:   10Dec2006
//
// Description:     This class implements a robot cell neighborhood.
//

// preprocessor directives
#include "Neighborhood.h"
#include <time.h>



// <constructors>

//
// Neighborhood()
// Last modified: 04Sep2006
//
// Default constructor that initializes
// this neighborhood to the appropriate values.
//
// Returns:     <none>
// Parameters:  <none>
//
Neighborhood::Neighborhood(): LinkedList<Neighbor>()
{
}   // Neighborhood()



//
// Neighborhood(nh)
// Last modified: 02Sep2006
//
// Copy constructor that copies the contents of the
// parameterized neighborhood into this neighborhood.
//
// Returns:     <none>
// Parameters:
//      nh      in/out      the neighborhood being copied
//
Neighborhood::Neighborhood(const Neighborhood &nh)
    : LinkedList<Neighbor>(nh)
{
}   // Neighborhood(const Neighborhood &)



//
// Neighborhood(r)
// Last modified: 02Sep2006
//
// Copy constructor that copies the contents of the
// parameterized list of relationships into this neighborhood.
//
// Returns:     <none>
// Parameters:
//      r       in/out      the list of relationships being copied
//
Neighborhood::Neighborhood(const LinkedList<Relationship> &r)
{
    for (GLint i = 0; i < r.getSize(); ++i) addNbr(r[i], State());
}   // Neighborhood(const LinkedList<Relationship> &)



//
// Neighborhood(s)
// Last modified: 02Sep2006
//
// Copy constructor that copies the contents of the
// parameterized list of states into this neighborhood.
//
// Returns:     <none>
// Parameters:
//      s       in/out      the list of states being copied
//
Neighborhood::Neighborhood(const LinkedList<State> &s)
{
    for (GLint i = 0; i < s.getSize(); ++i) addNbr(Relationship(), s[i]);
}   // Neighborhood(const LinkedList<State> &)



// <destructors>

//
// ~Neighborhood()
// Last modified: 02Sep2006
//
// Destructor that clears this neighborhood.
//
// Returns:     <none>
// Parameters:  <none>
//
Neighborhood::~Neighborhood()
{
}   // ~Neighborhood()



// <public mutator functions>

//
// bool addNbr(n)
// Last modified: 02Sep2006
//
// Attempts to add the parameterized neighbor to the neighborhood,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      n       in      the neighbor being added
//
bool Neighborhood::addNbr(const Neighbor n)
{
    return insertTail(n);
}   // addNbr(const Neighbor)



//
// bool addNbr(r, s)
// Last modified: 02Sep2006
//
// Attempts to add a neighbor to the neighborhood
// based on the parameterized relationship and state,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      r       in      the relationship of the neighbor being added
//      s       in      the state of the neighbor being added
//
bool Neighborhood::addNbr(const Relationship r, const State s)
{
    return addNbr(Neighbor(r, s));
}   // addNbr(const Relationship, const State)



//
// bool addNbr(id, s, desired, actual)
// Last modified: 02Sep2006
//
// Attempts to add a neighbor to the neighborhood
// based on the parameterized ID, state, and relationships,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      id          in  the ID of the neighbor being added
//      s           in  the state of the neighbor being added
//      desired     in  the desired relationship of the neighbor being added
//      actual      in  the actual relationship of the neighbor being added
//
bool Neighborhood::addNbr(const GLint  id,      const State  s,
                          const Vector desired, const Vector actual)
{
    return addNbr(Neighbor(id, s, desired, actual));
}   // addNbr(const GLint, const State, const Vector, const Vector)



//
// bool removeNbr(n)
// Last modified: 02Sep2006
//
// Attempts to remove the parameterized neighbor,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      n       in      the neighbor to remove
//
bool Neighborhood::removeNbr(const Neighbor n)
{
    return removeNbr(n.ID);
}   // removeNbr(const Neighbor)



//
// bool removeNbr(id)
// Last modified: 02Sep2006
//
// Attempts to remove the neighbor with the parameterized ID,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      id      in      the ID of the neighbor to remove
//
bool Neighborhood::removeNbr(const GLint id)
{
    Neighbor nbr;
    for (GLint i = 0; i < getSize(); ++i)
    {
        if (!getHead(nbr)) return false;
        if (nbr.ID == id)  return removeHead();
        ++(*this);
    }
    return false;
}   // removeNbr(const GLint)



// <virtual public accessor functions>

//
// LinkedList<Relationship> getRelationships()
// Last modified: 02Sep2006
//
// Returns the list of neighbor relationships.
//
// Returns:     list of neighbor relationships
// Parameters:  <none>
//
LinkedList<Relationship> Neighborhood::getRelationships()
{
    LinkedList<Relationship> rels;
    Neighbor currNbr;
    for (GLint i = 0; i < getSize(); ++i)
        if ((!getHead(currNbr)) || (!rels.insertTail(currNbr))) break;
        else ++(*this);
    return rels;
}   // getRelationships()



//
// LinkedList<State> getStates()
// Last modified: 02Sep2006
//
// Returns the list of neighbor states.
//
// Returns:     list of neighbor states
// Parameters:  <none>
//
LinkedList<State> Neighborhood::getStates()
{
    LinkedList<State> states;
    Neighbor currNbr;
    for (GLint i = 0; i < getSize(); ++i)
        if ((!getHead(currNbr)) || (!states.insertTail(currNbr))) break;
        else ++(*this);
    return states;
}   // getStates()



// <public accessor functions>

//
// Neighbor* getNbr(pos) const
// Last modified: 02Sep2006
//
// Returns the neighbor at the parameterized position in this neighborhood.
//
// Returns:     the neighbor at the parameterized position in this neighborhood
// Parameters:
//      pos     in      the position of the desired neighbor (default 0)
//
Neighbor* Neighborhood::getNbr(const GLint pos) const
{
    if ((pos < 0) || (pos > getSize())) return NULL;
    return &(*this)[pos];
}   // getNbr(const GLint) const



//
// LinkedList<Neighbor> getNbrs() const
// Last modified: 02Sep2006
//
// Returns the list of neighbors in this neighborhood.
//
// Returns:     list of neighbors in this neighborhood
// Parameters:  <none>
//
LinkedList<Neighbor> Neighborhood::getNbrs() const
{
    return *this;
}   // getNbrs() const



//
// GLint getNNbrs() const
// Last modified: 02Sep2006
//
// Returns the number of neighbors in this neighborhood.
//
// Returns:     the number of neighbors in this neighborhood
// Parameters:  <none>
//
GLint Neighborhood::getNNbrs() const
{
    return getSize();
}   // getNNbrs() const



// <public utility functions>

//
// bool updateNbr(n, s)
// Last modified: 07Sep2006
//
// Updates the state of the parameterized neighbor,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      n       in/out  the neighbor being updated
//      s       in/out  the updated state of the neighbor
//
bool Neighborhood::updateNbr(Neighbor &n, const State &s)
{
    n = s;
    return true;
}   // updateNbr(Neighbor* &, const State &)



//
// bool updateNbr(id, s)
// Last modified: 07Sep2006
//
// Updates the state of the neighbor with the parameterized ID,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      id      in      the ID of the neighbor being updated
//      s       in/out  the updated state of the neighbor
//
bool Neighborhood::updateNbr(const GLint id, const State &s)
{
    Neighbor *nbr = nbrWithID(id);
    if (nbr == NULL) return false;
    return updateNbr(*nbr, s);
}   // updateNbr(const GLint, const State &)



//
// bool isNbr(n)
// Last modified: 02Sep2006
//
// Returns true if the parameterized neighbor
// is in this neighborhood, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      n       in      the neighbor to find
//
bool Neighborhood::isNbr(const Neighbor n)
{
    return isNbr(n.ID);
}   // isNbr(const Neighbor)



//
// bool isNbr(id)
// Last modified: 02Sep2006
//
// Returns true if the neighbor with the parameterized ID
// is in this neighborhood, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      id      in      the ID of the neighbor to find
//
bool Neighborhood::isNbr(const GLint id)
{
    Neighbor nbr;
    for (GLint i = 0; i < getSize(); ++i)
    {
        getHead(nbr);
        if (nbr.ID == id) return true;
        else              ++(*this);
    }
    return false;
}   // isNbr(const GLint)


//
// void clearNbrs()
// Last modified: 02Sep2006
//
// Clears the neighbors in this neighborhood.
//
// Returns:     <none>
// Parameters:  <none>
//
void Neighborhood::clearNbrs()
{
    clear();
}   // clearNbrs()



// <public single neighbor member functions>

//
// Neighbor* anyNbr()
// Last modified: 03Sep2006
//
// Returns a random neighbor in this neighborhood.
//
// Returns:     random neighbor in this neighborhood
// Parameters:  <none>
//
Neighbor* Neighborhood::anyNbr()
{
    srand(time(NULL));
    if (!isEmpty()) return &(*this)[irand(0, getSize())];
    return NULL;
}   // anyNbr()



//
// Neighbor* firstNbr()
// Last modified: 03Sep2006
//
// Returns the first neighbor in this neighborhood.
//
// Returns:     the first neighbor in this neighborhood
// Parameters:  <none>
//
Neighbor* Neighborhood::firstNbr()
{
    if (getHead(Neighbor())) return &(*this)[0];
    return NULL;
}   // firstNbr(Neighborhood &nh)



//
// Neighbor* secondNbr()
// Last modified: 03Sep2006
//
// Returns the second neighbor in this neighborhood.
//
// Returns:     the second neighbor in this neighborhood
// Parameters:  <none>
//
Neighbor* Neighborhood::secondNbr()
{
    if (getHeadNext(Neighbor())) return &(*this)[1];
    return NULL;
}   // secondNbr()



//
// Neighbor* lastNbr()
// Last modified: 03Sep2006
//
// Returns the last neighbor in this neighborhood.
//
// Returns:     the last neighbor in this neighborhood
// Parameters:  <none>
//
Neighbor* Neighborhood::lastNbr()
{
    if (getTail(Neighbor())) return &(*this)[getSize() - 1];
    return NULL;
}   // lastNbr()



//
// Neighbor* closestNbr(v)
// Last modified: 03Sep2006
//
// Returns the closest neighbor in this neighborhood
// as determined by the parameterized difference vector.
//
// Returns:     the closest neighbor in this neighborhood
// Parameters:
//      v       in      the difference vector
//
Neighbor* Neighborhood::closestNbr(const Vector v)
{
    Neighbor currNbr;
    GLfloat  minDist  = 0.0f, currDist = 0.0f;
    GLint    minIndex = ID_NO_NBR;
    for (GLint i = 0; i < getSize(); ++i)
    {
        if (!getHead(currNbr)) return NULL;
        if ((currNbr.ID != ID_NO_NBR) &&
            (((currDist = (currNbr.relActual - v).norm()) < minDist) ||
             (minIndex == ID_NO_NBR)))
        {
            minDist   = currDist;
            minIndex  = i;
        }
        ++(*this);
    }
    return ((minIndex < 0) || (minIndex > getSize()))
            ? NULL : &(*this)[minIndex];
}   // closestNbr(const Vector)



//
// Neighbor* furthestNbr(v)
// Last modified: 03Sep2006
//
// Returns the furthest neighbor in this neighborhood
// as determined by the parameterized difference vector.
//
// Returns:     the furthest neighbor in this neighborhood
// Parameters:
//      v       in      the difference vector
//
Neighbor* Neighborhood::furthestNbr(const Vector v)
{
    Neighbor currNbr;
    GLfloat  maxDist  = 0.0f, currDist = 0.0f;
    GLint    maxIndex = ID_NO_NBR;
    for (GLint i = 0; i < getSize(); ++i)
    {
        if (!getHead(currNbr)) return NULL;
        if ((currNbr.ID != ID_NO_NBR) &&
            (((currDist = (currNbr.relActual - v).norm()) > maxDist) ||
             (maxIndex == ID_NO_NBR)))
        {
            maxDist   = currDist;
            maxIndex  = i;
        }
        ++(*this);
    }
    return ((maxIndex < 0) || (maxIndex > getSize()))
            ? NULL : &(*this)[maxIndex];
}   // furthestNbr(const Vector)



// <public single neighbor property member functions>

//
// Neighbor* nbrWithID(id)
// Last modified: 07Sep2006
//
// Returns the neighbor (in this neighborhood) with the parameterized ID.
//
// Returns:     the neighbor with the parameterized ID
// Parameters:
//      id      in      the ID of the neighbor to find
//
Neighbor* Neighborhood::nbrWithID(const GLint id)
{
    Neighbor currNbr;
    for (GLint i = 0; i < getSize(); ++i)
    {
        if (!getHead(currNbr)) break;
        if (currNbr.ID == id)  return &(*this)[0];
        ++(*this);
    }
    return NULL;
}   // nbrWithID(const GLint)



//
// Neighbor* nbrWithGradient(grad)
// Last modified: 07Sep2006
//
// Returns the neighbor (in this neighborhood) with the parameterized gradient.
//
// Returns:     the neighbor with the parameterized gradient
// Parameters:
//      gradient    in  the gradient of the neighbor to find
//
Neighbor* Neighborhood::nbrWithGradient(const Vector grad)
{
    Neighbor currNbr;
    for (GLint i = 0; i < getSize(); ++i)
    {
        if (!getHead(currNbr))        break;
        if (currNbr.gradient == grad) return &(*this)[0];
        ++(*this);
    }
    return NULL;
}   // nbrWithGradient(const Vector)



//
// Neighbor* nbrWithMinGradient(v)
// Last modified: 03Sep2006
//
// Returns the neighbor (in this neighborhood) with the minimum gradient
// distance as determined by the parameterized difference vector.
//
// Returns:     the neighbor with the minimum gradient
// Parameters:
//      v       in      the difference vector
//
Neighbor* Neighborhood::nbrWithMinGradient(const Vector v)
{
    Neighbor currNbr;
    GLfloat  minGrad  = 0.0f, currGrad = 0.0f;
    GLint    minIndex = ID_NO_NBR;
    for (GLint i = 0; i < getSize(); ++i)
    {
        if (!getHead(currNbr)) return NULL;
        if ((currNbr.ID != ID_NO_NBR) &&
            (((currGrad = (currNbr.gradient - v).norm())
              < minGrad) || (minIndex == ID_NO_NBR)))
        {
            minGrad   = currGrad;
            minIndex  = i;
        }
        ++(*this);
    }
    return ((minIndex < 0) || (minIndex > getSize()))
            ? NULL : &(*this)[minIndex];
}   // nbrWithMinGradient(const Vector)



//
// Neighbor* nbrWithMaxGradient(v)
// Last modified: 03Sep2006
//
// Returns the neighbor (in this neighborhood) with the maximum gradient
// distance as determined by the parameterized difference vector.
//
// Returns:     the neighbor with the maximum gradient
// Parameters:
//      v       in      the difference vector
//
Neighbor* Neighborhood::nbrWithMaxGradient(const Vector v)
{
    Neighbor currNbr;
    GLfloat  maxGrad  = 0.0f, currGrad = 0.0f;
    GLint    maxIndex = ID_NO_NBR;
    for (GLint i = 0; i < getSize(); ++i)
    {
        if (!getHead(currNbr)) return NULL;
        if ((currNbr.ID != ID_NO_NBR) &&
            (((currGrad = (currNbr.gradient - v).norm())
              > maxGrad) || (maxIndex == ID_NO_NBR)))
        {
            maxGrad   = currGrad;
            maxIndex  = i;
        }
        ++(*this);
    }
    return ((maxIndex < 0) || (maxIndex > getSize()))
            ? NULL : &(*this)[maxIndex];
}   // nbrWithMaxGradient(const Vector)



// <public neighbor list member functions>

//
// void sortByID()
// Last modified: 03Sep2006
//
// Sorts the neighborhood based upon neighbor ID.
//
// Returns:     <none>
// Parameters:  <none>
//
void Neighborhood::sortByID()
{
    for (GLint i = 0; i < getSize() - 1; ++i)
        for (GLint j = i; j < getSize(); ++j)
            if (getNbr(i)->ID > getNbr(j)->ID)
                swap(*getNbr(i), *getNbr(j));
}   // sortByID()



//
// void sortByGradient(v)
// Last modified: 03Sep2006
//
// Sorts the neighborhood based upon neighbor gradient
// as determined by the parameterized difference vector.
//
// Returns:     <none>
// Parameters:
//      v       in      the difference vector
//
void Neighborhood::sortByGradient(const Vector v)
{
    for (GLint i = 0; i < getSize() - 1; ++i)
        for (GLint j = i; j < getSize(); ++j)
            if ((getNbr(i)->gradient - v).norm() >
                (getNbr(j)->gradient - v).norm())
                swap(*getNbr(i), *getNbr(j));
}   // sortByGradient(const Vector)



//
// void sortByDistance(v)
// Last modified: 03Sep2006
//
// Sorts the neighborhood based upon neighbor distance
// as determined by the parameterized difference vector.
//
// Returns:     <none>
// Parameters:
//      v       in      the difference vector
//
void Neighborhood::sortByDistance(const Vector v)
{
    for (GLint i = 0; i < getSize() - 1; ++i)
        for (GLint j = i; j < getSize(); ++j)
            if ((getNbr(i)->relActual - v).norm() >
                (getNbr(j)->relActual - v).norm())
                swap(*getNbr(i), *getNbr(j));
}   // sortByDistance(const Vector)



//
// void sortByAngle(v)
// Last modified: 10Dec2006
//
// Sorts the neighborhood based upon the angle of the
// neighbor-relative and the parameterized difference vector.
//
// Returns:     <none>
// Parameters:
//      v       in      the difference vector
//
void Neighborhood::sortByAngle(const Vector v)
{
    for (GLint i = 0; i < getSize() - 1; ++i)
        for (GLint j = i; j < getSize(); ++j)
            if ((getNbr(i)->relActual - v).angle() >
                (getNbr(j)->relActual - v).angle())
                swap(*getNbr(i), *getNbr(j));
}   // sortByAngle(const Vector)



//
// void sortByAbsAngle(v)
// Last modified: 03Sep2006
//
// Sorts the neighborhood based upon the absolute value of the angle
// of the neighbor-relative and the parameterized difference vector.
//
// Returns:     <none>
// Parameters:
//      v       in      the difference vector
//
void Neighborhood::sortByAbsAngle(const Vector v)
{
    for (GLint i = 0; i < getSize() - 1; ++i)
        for (GLint j = i; j < getSize(); ++j)
            if (abs((getNbr(i)->relActual - v).angle()) >
                abs((getNbr(j)->relActual - v).angle()))
                swap(*getNbr(i), *getNbr(j));
}   // sortByAbsAngle(const Vector)
