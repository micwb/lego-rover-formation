//
// Filename:        "Queue.h"
// Last modified:   26Jun2006
//
// This library of classes describes and
// implements a templated queue of nodes.
//



//
// <Queue>
// Last modified:   14May2006
//
// This class describes a templated queue of nodes.
//

// preprocessor directives
#ifndef QUEUE
#define QUEUE
#include "LinkedList.h"
using namespace std;

template <class T>
class Queue: protected LinkedList<T>
{

    public:

        // <constructors>
        Queue();
        Queue(const LinkedList<T> &list);

        // <destructors>
        ~Queue();

        // <virtual public mutator functions>
        virtual bool enqueue(const T item);
        virtual bool dequeue();
        virtual bool dequeue(T &item);
        virtual void clear();

        // <public accessor functions>
        bool getFront(T &item) const;
        int  getSize()         const;
        bool isEmpty()         const;
};  // Queue<T>



//
// <Queue>
// Last modified: 14May2006
//
// This class implements a templated queue of nodes.
//



// <constructors>

//
// Queue()
// Last modified: 14May2006
//
// Default constructor that sets the head pointer to NULL and the size to 0.
//
// Returns:     <none>
// Parameters:  <none>
//
template <class T>
Queue<T>::Queue(): LinkedList<T>()
{
}   // Queue()



//
// Queue(queue)
// Last modified: 14May2006
//
// Copy constructor that copies the contents
// of the parameterized list into this queue.
//
// Returns:     <none>
// Parameters:
//      queue    in/out      the queue being copied
//
template <class T>
Queue<T>::Queue(const LinkedList<T> &list): LinkedList<T>(list)
{
}   // Queue(const LinkedList<T> &)



// <destructors>

//
// ~Queue()
// Last modified: 14May2006
//
// Destructor that clears this queue.
//
// Returns:     <none>
// Parameters:  <none>
//
template <class T>
Queue<T>::~Queue()
{
    LinkedList<T>::~LinkedList();
}   // ~Queue()



// <public mutator functions>

//
// bool enqueue(item, pos)
// Last modified: 14May2006
//
// Attempts to enqueue an item into the queue,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    in      the item being enqueued
//
template <class T>
bool Queue<T>::enqueue(const T item)
{
    return LinkedList<T>::insertTail(item);
}   // enqueue(const T)



//
// bool dequeue()
// Last modified: 14May2006
//
// Attempts to dequeue an item from the queue,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
template <class T>
bool Queue<T>::dequeue()
{
    return LinkedList<T>::removeHead();
}   // dequeue()



//
// bool dequeue(item)
// Last modified: 14May2006
//
// Attempts to dequeue an item from the queue,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    out     set to the value of the front item
//
template <class T>
bool Queue<T>::dequeue(T &item)
{
    return LinkedList<T>::removeHead(item);
}   // dequeue(T &)



//
// void clear()
// Last modified: 14May2006
//
// Clears the queue.
//
// Returns:     <none>
// Parameters:  <none>
//
template <class T>
void Queue<T>::clear()
{
    LinkedList<T>::clear();
}   // clear()



// <public accessor functions>

//
// bool getFront(item) const
// Last modified: 14May2006
//
// Attempts to retrieve the value of the front item
// in the queue, assigning it to the given parameter,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    out     set to the value of the front item
//
template <class T>
bool Queue<T>::getFront(T &item) const
{
    return LinkedList<T>::getHead(item);
}   // getFront(T &) const



//
// bool getSize() const
// Last modified: 14May2006
//
// Returns the number of nodes in the queue.
//
// Returns:     the number of nodes in the queue
// Parameters:  <none>
//
template <class T>
int Queue<T>::getSize() const
{
    return LinkedList<T>::getSize();
}   // getSize() const



//
// bool isEmpty() const
// Last modified: 14May2006
//
// Returns true if the queue is empty, false otherwise.
//
// Returns:     true if the queue is empty, false otherwise
// Parameters:  <none>
//
template <class T>
bool Queue<T>::isEmpty() const
{
    return LinkedList<T>::isEmpty();
}   // isEmpty() const
#endif
