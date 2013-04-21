//
// Filename:        "LinkedList.h"
//
// Programmer:      Ross Mead
// Last modified:   05Sep2006
//
// This library of classes describes and
// implements a templated list of nodes.
//



//
// <Node>
// Last modified:   07Jun2006
//
// This class implements a templated node.
//

// preprocessor directives
#ifndef NODE
#define NODE
#include <iostream>
using namespace std;

template <class T> class LinkedList;
template <class T>
class Node
{

    protected:

        // <protected data members>
        T     item;
        Node *next, *prev;

        // <constructors>
        Node(): next(NULL), prev(NULL) {};
        Node(const T i, Node<T> *n = NULL, Node<T> *p = NULL)
            : item(i), next(n), prev(p) {};
        
        // <protected friend classes>
        friend class LinkedList<T>;
};  // Node
#endif



//
// <LinkedList>
// Last modified:   15Jul2006
//
// This class describes a templated list of nodes.
//

// preprocessor directives
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

template <class T>
class LinkedList
{

    public:

        // <constructors>
        LinkedList();
        LinkedList(const LinkedList<T> &list);

        // <destructors>
        ~LinkedList();

        // <virtual public mutator functions>
        virtual bool insert(const T item, const int pos = 0);
        virtual bool insertHead(const T item);
        virtual bool insertTail(const T item);
        virtual bool remove(const int pos = 0);
        virtual bool removeHead();
        virtual bool removeHead(T &item);
        virtual bool removeTail();
        virtual bool removeTail(T &item);
        virtual void clear();

        // <virtual public accessor functions>
        virtual bool getHead(T &item)     const;
        virtual bool getHeadNext(T &item) const;
        virtual bool getHeadPrev(T &item) const;
        virtual bool getTail(T &item)     const;
        virtual int  getSize()            const;
        virtual bool isEmpty()            const;

        // <virtual overloaded operators>
        virtual LinkedList<T>& operator =(const LinkedList<T> &list);
        virtual LinkedList<T>& operator ++();
        virtual LinkedList<T>& operator --();
        virtual T&             operator [](const int pos) const;

    protected:

        // <protected data members>
        Node<T> *head;
        int      size;

        // <virtual protected utility functions>
        virtual Node<T>* getNode(const int pos) const;
};  // LinkedList<T>



//
// <LinkedList>
// Last modified: 05Sep2006
//
// This class implements a templated list of nodes.
//

// preprocessor directives
#include <cassert>
using namespace std;



// <constructors>

//
// LinkedList()
// Last modified: 12May2006
//
// Default constructor that sets the head pointer to NULL and the size to 0.
//
// Returns:     <none>
// Parameters:  <none>
//
template <class T>
LinkedList<T>::LinkedList(): head(NULL), size(0)
{
}   // LinkedList()



//
// LinkedList(list)
// Last modified: 14May2006
//
// Copy constructor that copies the contents
// of the parameterized list into this list.
//
// Returns:     <none>
// Parameters:
//      list    in/out      the list being copied
//
template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> &list): head(NULL), size(0)
{
    *this = list;   // copy contents of the parameterized list into this list
}   // LinkedList(const LinkedList<T> &)



// <destructors>

//
// ~LinkedList()
// Last modified: 12May2006
//
// Destructor that clears this list.
//
// Returns:     <none>
// Parameters:  <none>
//
template <class T>
LinkedList<T>::~LinkedList()
{
    clear();
}   // ~list()



// <virtual public mutator functions>

//
// bool insert(item, pos)
// Last modified: 12May2006
//
// Attempts to insert an item at the parameterized position
// in the list, inserting at the head by default, returning
// true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    in      the item being inserted
//      pos     in      the position to insert at (default head)
//
template <class T>
bool LinkedList<T>::insert(const T item, const int pos)
{
    if ((pos < 0) || (pos > getSize())) return false;
    if (isEmpty())
        if ((head = new Node<T>(item)) != NULL)
        {
            head->next = head->prev = head;
            ++size;
            return true;
        }
        else return false;
    Node<T> *temp      = getNode((pos == getSize()) ? 0 : pos);
    if (temp == NULL) return false;
    Node<T> *node      = new Node<T>(item, temp, temp->prev);
    if (node == NULL) return false;
    temp->prev->next   = node;
    temp->prev         = node;
    if (pos == 0) head = node;
    ++size;
    return true;
}   // insert(const T, const int)



//
// bool insertHead(item)
// Last modified: 12May2006
//
// Attempts to insert an item at the head of the list,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    in      the item being inserted
//
template <class T>
bool LinkedList<T>::insertHead(const T item)
{
    return insert(item, 0);
}   // insertHead(const T)



//
// bool insertTail(item)
// Last modified: 12May2006
//
// Attempts to insert an item at the tail (head-previous)
// of the list, returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    in      the item being inserted
//
template <class T>
bool LinkedList<T>::insertTail(const T item)
{
    return insert(item, getSize());
}   // insertTail(const T)



//
// bool remove(pos)
// Last modified: 14May2006
//
// Attempts to remove the item at the parameterized position
// from the list, removing the head by default, returning
// true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      pos     in      the position to remove (default head)
//
template <class T>
bool LinkedList<T>::remove(const int pos)
{
    Node<T> *curr            = getNode(pos);
    if (curr == NULL) return false;
    if (getSize() == 1) head = NULL;
    else
    {
        curr->prev->next     = curr->next;
        curr->next->prev     = curr->prev;
        if ((pos == 0) || (pos == getSize())) head = head->next;
    }
    delete curr;
    curr                     = NULL;
    --size;
    return true;
}   // remove(const int)



//
// bool removeHead()
// Last modified: 12May2006
//
// Attempts to remove the head item from the list,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
template <class T>
bool LinkedList<T>::removeHead()
{
    return remove(0);
}   // removeHead()



//
// bool removeHead(item)
// Last modified: 14May2006
//
// Attempts to remove the head item from the list,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    out     set to the value of the head item
//
template <class T>
bool LinkedList<T>::removeHead(T &item)
{
    return getHead(item) && removeHead();
}   // removeHead(T &)



//
// bool removeTail()
// Last modified: 13May2006
//
// Attempts to remove the tail (head-previous) item from the list,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
template <class T>
bool LinkedList<T>::removeTail()
{
    return remove(getSize() - 1);
}   // removeTail()



//
// bool removeTail(item)
// Last modified: 14May2006
//
// Attempts to remove the tail (head-previous) item from the list,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    out     set to the value of the head item
//
template <class T>
bool LinkedList<T>::removeTail(T &item)
{
    return getTail(item) && removeTail();
}   // removeTail(T &)



//
// void clear()
// Last modified: 12May2006
//
// Clears the list.
//
// Returns:     <none>
// Parameters:  <none>
//
template <class T>
void LinkedList<T>::clear()
{
    while (removeHead());
}   // clear()



// <virtual public accessor functions>

//
// bool getHead(item) const
// Last modified: 16May2006
//
// Attempts to retrieve the value of the head item
// in the list, assigning it to the given parameter,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    out     set to the value of the head item
//
template <class T>
bool LinkedList<T>::getHead(T &item) const
{
    if (isEmpty()) return false;
    item = head->item;          // assign value to the given parameter
    return true;
}   // getHead(T &) const



//
// bool getHeadNext(item) const
// Last modified: 16May2006
//
// Attempts to retrieve the value of the head-next item
// in the list, assigning it to the given parameter,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    out     set to the value of the head-next item
//
template <class T>
bool LinkedList<T>::getHeadNext(T &item) const
{
    if (isEmpty()) return false;
    item = head->next->item;    // assign value to the given parameter
    return true;
}   // getHeadNext(T &) const



//
// bool getPrevNext(item) const
// Last modified: 16May2006
//
// Attempts to retrieve the value of the head-previous item
// in the list, assigning it to the given parameter,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    out     set to the value of the head-previous item
//
template <class T>
bool LinkedList<T>::getHeadPrev(T &item) const
{
    if (isEmpty()) return false;
    item = head->prev->item;    // assign value to the given parameter
    return true;
}   // getHeadPrev(T &) const



//
// bool getTail(item) const
// Last modified: 13May2006
//
// Attempts to retrieve the value of the tail (head-previous)
// item in the list, assigning it to the given parameter,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      item    out     set to the value of the head-previous item
//
template <class T>
bool LinkedList<T>::getTail(T &item) const
{
    return getHeadPrev(item);
}   // getTail(T &) const



//
// bool getSize() const
// Last modified: 12May2006
//
// Returns the number of nodes in the list.
//
// Returns:     the number of nodes in the list
// Parameters:  <none>
//
template <class T>
int LinkedList<T>::getSize() const
{
    return size;
}   // getSize() const



//
// bool isEmpty() const
// Last modified: 12May2006
//
// Returns true if the list is empty, false otherwise.
//
// Returns:     true if the list is empty, false otherwise
// Parameters:  <none>
//
template <class T>
bool LinkedList<T>::isEmpty() const
{
    return getSize() == 0;
}   // isEmpty() const



// <virtual overloaded operators>

//
// LinkedList<T>& =(list)
// Last modified: 05Sep2006
//
// Copies the contents of the parameterized list into this list.
//
// Returns:     this list
// Parameters:
//      list    in/out  the list being copied
//
template <class T>
LinkedList<T>& LinkedList<T>::operator =(const LinkedList &list)
{
    clear();    // clears this list

    // copies the contents of the parameterized list to this list
    Node<T> *curr = list.head;
    for (int i = 0; i < list.getSize(); ++i)
    {
        insert(curr->item, i);
        curr = curr->next;
    }
    return *this;
}   // =(const LinkedList &)



//
// LinkedList<T>& ++()
// Last modified: 12Aug2006
//
// Moves the head to head-next.
//
// Returns:     this list
// Parameters:  <none>
//
template <class T>
LinkedList<T>& LinkedList<T>::operator ++()
{
    if (head->next != NULL) head = head->next;
    return *this;
}   // ++()



//
// LinkedList<T>& --()
// Last modified: 12Aug2006
//
// Moves the head to head-next.
//
// Returns:     this list
// Parameters:  <none>
//
template <class T>
LinkedList<T>& LinkedList<T>::operator --()
{
    if (head->prev != NULL) head = head->prev;
    return *this;
}   // --()



//
// T& [] (pos)
// Last modified: 12Aug2006
//
// Provides array-like access of list items.
//
// Returns:     the list item at the parameterized position
// Parameters:
//      pos     in      the position of the desired item
//
template <class T>
T& LinkedList<T>::operator [](const int pos) const
{
    assert((pos >= 0) && (pos <= getSize()));
    return getNode(pos)->item;
}   // [](const int) const



// <virtual protected utility functions>

//
// Node<T>& getNode(pos)
// Last modified: 12May2006
//
// Returns the node at the parameterized position.
//
// Returns:     <none>
// Parameters:
//      pos     in      the position of the desired node
//
template <class T>
Node<T>* LinkedList<T>::getNode(const int pos) const
{
    if ((pos < 0) || (pos > getSize())) return NULL;
    Node<T> *curr = head;
    if (pos < (getSize() + 1) / 2)
        for (int i = 0;         i < pos; ++i) curr = curr->next;
    else
        for (int i = getSize(); i > pos; --i) curr = curr->prev;
    return curr;
}   // getNode(const int) const
#endif
