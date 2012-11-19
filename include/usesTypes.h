#ifndef _USES_TYPES_H_
#define _USES_TYPES_H_
#include <iostream>

using std::cout; 
using std::endl;

struct Point
{
	float x, y;
	void Print(){cout<<"{"<<x<<"; "<<y<<"}";}
	Point(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
	Point(){x=y=0;}
};

struct Segment
{
	Point * vertex[2];
	bool fullInfo;
	int a,b,c; // ay + bx + c == 0
	void CalculSegEq()
	{	
		if (fullInfo) return; 
		a = vertex[0]->x - vertex[1]->x; 
		b = vertex[1]->y - vertex[0]->y;
		c = vertex[1]->x * vertex[0]->y - vertex[0]->x * vertex[1]->y;
		fullInfo = true;
	}
	void Print()
	{
		vertex[0]->Print();cout<<"----";vertex[1]->Print();
		if(fullInfo)
			cout<<endl<<"a "<<a<<" b "<<b<<" c "<<c;
	}
};

#endif