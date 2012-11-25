#ifndef _NODE_H_
#define _NODE_H_

/// abstract class. Need for stack, list.
template <class T>
struct NodeBack
{
	T  data;
	NodeBack<T> * prev;
	void NodeInit(T _data) {data = _data; prev = 0;}
	NodeBack(T _data) {data = _data; prev = 0;};
	~NodeBack() {};
};

#endif