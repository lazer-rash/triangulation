#ifndef _NODE_H_
#define _NODE_H_

#include <stdlib.h>

/// abstract class. Need for stack, list.

template <class T>
struct Node
{
	T  data;
	Node<T> * next;
	Node<T> * prev;
	void NodeInit(T _data) {data = _data; next = prev = NULL;}
	Node(T _data) {data = _data; next = prev = NULL;};
	~Node() {};
};

template <class T>
struct NodeBack
{
	T  data;
	NodeBack<T> * prev;
	void NodeInit(T _data) {data = _data; prev = NULL;}
	NodeBack(T _data) {data = _data; prev = NULL;};
	~NodeBack() {};
};
#endif