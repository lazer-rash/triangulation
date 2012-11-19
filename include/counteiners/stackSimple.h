#ifndef _STACK_SIMPLE_H_
#define _STACK_SIMPLE_H_

#include "node.h"
#include <stdlib.h>

template <class T>
class StackSimple
{
private:
	NodeBack<T> * current;
public:
	void Push(T _data);
	T Top() {return current->data;}
	void Pop();
	bool IsEmpty() {return current == NULL;}
	StackSimple() {current = NULL;}
	~StackSimple();
};

template <class T>
void StackSimple<T>::Push(T _data)
{
	NodeBack<T> * tmp = new NodeBack<T>(_data);
	tmp->prev = current;
	current = tmp;
}

template <class T>
void StackSimple<T>::Pop()
{
	NodeBack<T> * tmp = current->prev;
	delete current;
	current = tmp;
}

template <class T>
StackSimple<T>::~StackSimple()
{
	NodeBack<T> * tmp;
	while(current != NULL)
	{
		tmp = current->prev;
		delete current;
		current = tmp; 
	}
}

#endif
