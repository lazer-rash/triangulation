#ifndef _OBJECT_POOL_H_
#define _OBJECT_POOL_H_

#include "stackSimple.h"
#include <iostream>
using namespace std;

template <class T> struct MemSheet
{
	T * data;
	void FreeMemSheet(){free(data);}
	void MemSheetInit(int size)
	{
		data = (T *) malloc(size * sizeof(T));
	}
};

template <class T> class ObjectPool
{
private:
	StackSimple<MemSheet<T> *> allocMem;
	int countSh;
	int size;
	int used;
	StackSimple<NodeBack<T *> *> allocNodeMem;
	NodeBack<T *> * currentFree;
	NodeBack<T *> * currentBusy;

	void AddSheet();
public:
	int GetCount(){return countSh;}
	int GetObjCnt(){return used;}
	T * NewObj();
	void DelObj(T * _data);

	void SetSize(int _size){size = _size;}
	ObjectPool(int _size);
	ObjectPool();
	~ObjectPool();
};

template <class T>
void ObjectPool<T>::AddSheet()
{
	MemSheet<T> * newPage = (MemSheet<T> *) malloc(sizeof(MemSheet<T>));
	newPage->MemSheetInit(size);
	allocMem.Push(newPage);
	NodeBack<T *> * newFreeRef = (NodeBack<T *> *) malloc(size * sizeof(NodeBack<T *>));
	allocNodeMem.Push(newFreeRef);
	for (int i = 0; i < size - 1; ++i)
		newFreeRef[i].prev = newFreeRef + i + 1;
	newFreeRef[size - 1].prev = currentFree;
	for (int i = 0; i < size; ++i) newFreeRef[i].data = newPage->data + i;
	currentFree = newFreeRef;
	countSh++;
}

template <typename T>
T * ObjectPool<T>::NewObj()
{
	used ++;
	if (currentFree == NULL) AddSheet();
	NodeBack<T * > * tmp = currentFree->prev;
	currentFree->prev = currentBusy;
	currentBusy = currentFree;
	currentFree = tmp;	
	return currentBusy->data;
}

template <typename T>
void ObjectPool<T>::DelObj(T * data)
{
	used --;
	NodeBack<T *> * tmp = currentBusy->prev;
	currentBusy->NodeInit(data);
	currentBusy->prev = currentFree;
	currentFree = currentBusy;
	currentBusy = tmp;
}

template <typename T>
ObjectPool<T>::ObjectPool(int _size)
{
	size = _size;
	countSh = 0;
	currentFree = NULL;
	currentBusy = NULL;
	used = 0;
}

template <typename T>
ObjectPool<T>::ObjectPool()
{
	size = 0;
	countSh = 0;
	currentFree = NULL;
	currentBusy = NULL;
	used = 0;
}

template <typename T>
ObjectPool<T>::~ObjectPool()
{
	while(!allocMem.IsEmpty())
	{	
		(allocMem.Top())->FreeMemSheet();
		allocMem.Pop();
	}

	while(!allocNodeMem.IsEmpty())
	{	
		(free(allocNodeMem.Top()));
		allocNodeMem.Pop();
	}
}

#endif