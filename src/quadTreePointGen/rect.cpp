#include "rect.h"
#include <mathUtils.h>

void Rect::Print()
{
	cout<< "ref "<<this<<endl;
	cout<<"  age: "<<age<<endl;
	cout<<"  size: "<<size<<endl;
	cout<<"  vertexes: "<<endl;
	cout<<"  ";
	for(int i = 0; i < 4; i++)
	{
		vertex[i]->Print();
		cout<<"  ";
	}
	cout<<endl;	

	NodeBack<Segment *> * tmp = firstSeg;
	cout<<"  borders: "<<endl;
	cout<<"  ";
	while(tmp != 0)
	{
		tmp->data->Print();
		tmp = tmp->prev;
		cout<<endl<<"  ";
	}
	cout<<endl;	

	NodeBack<Rect *> * tmp1 = firstNeighb;
	cout<<"  neigbh: "<<endl;
	while(tmp1 != 0)
	{
		cout<<" ref "<<tmp1->data<<" age "<<tmp1->data->age<<endl;
		tmp1 = tmp1->prev;
	}
	cout<<endl;	
}

bool IsPointInRect(Rect * rect, Point * point)
{
	if ((rect->vertex[0]->x > point->x) || (rect->vertex[1]->x < point->x))
		return false;
	if ((rect->vertex[3]->y > point->y) || (rect->vertex[0]->y < point->y))
		return false;
	return true;
}

bool IsNeigbhour(Rect * rect1, Rect * rect2)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (rect1->vertex[i] == rect2->vertex[j])
				return true;
		}

	float cx1, cx2, cy1,cy2, halfw1, halfw2, halfh1, halfh2;
	//центры прямоугольников
	halfw1 = (rect1->vertex[2]->x - rect1->vertex[3]->x) / 2;
	halfh1 = (rect1->vertex[0]->y - rect1->vertex[3]->y) / 2;
	halfw2 = (rect2->vertex[2]->x - rect2->vertex[3]->x) / 2;
	halfh2 = (rect2->vertex[0]->y - rect2->vertex[3]->y) / 2;
	cx1 = rect1->vertex[3]->x + halfw1;
	cy1 = rect1->vertex[3]->y + halfh1;
	cx2 = rect2->vertex[3]->x + halfw2;
	cy2 = rect2->vertex[3]->y + halfh2;

	//для соседства прямоугольников нужно fabs(cx1 - cx2) == (halfw1 + halfw2)
	//но из-за погрешностей 
	//расстояние по горизонтали между центрами
	if(fabs(cx1 - cx2) > 1.1 * (halfw1 + halfw2)) return false;
	if(fabs(cy1 - cy2) > 1.1 * (halfh1 + halfh2)) return false;

	return true;
}

bool IsSegmetCrosRect(Rect * rect,Segment * seg)
{
	if(IsPointInRect(rect, seg->vertex[0]) || IsPointInRect(rect, seg->vertex[1]))
	return true;

	int sideArr[4];

    //ay + bx + c == 0
	seg->CalculSegEq();

	//положение вершин относительно отрезка
	float tmp;
	for (int i = 0; i < 4; ++i)
	{
		tmp = seg->a * rect->vertex[i]->y + seg->b * rect->vertex[i]->x + seg->c;	
	 	sideArr[i] = Sign(tmp);
	}

	bool btmp = true; // по одну сторону?
	for (int i = 0; i < 3; ++i)
	{
	 	if(sideArr[3] != sideArr[i])
	 	{
	 		btmp =false;
	 		break;
	 	}
	}
	if(btmp) return false;

	//расположенение концов отрезка тносительно ребра 3-0
	float tmp1 = seg->vertex[0]->x - rect->vertex[0]->x;
	tmp = seg->vertex[1]->x - rect->vertex[0]->x;
	sideArr[0] = (Sign(tmp1) == Sign(tmp));
	//1-2
	tmp1 = seg->vertex[0]->x - rect->vertex[2]->x;
	tmp = seg->vertex[1]->x - rect->vertex[2]->x;
	sideArr[2] = (Sign(tmp1) == Sign(tmp));
	//0-1
    tmp1 = seg->vertex[0]->y - rect->vertex[0]->y;
	tmp = seg->vertex[1]->y - rect->vertex[0]->y;
	sideArr[1] = (Sign(tmp1) == Sign(tmp));
	//2-3
	tmp1 = seg->vertex[0]->y - rect->vertex[2]->y;
	tmp = seg->vertex[1]->y - rect->vertex[2]->y;
	sideArr[3] = (Sign(tmp1) == Sign(tmp));

	int sum = 0;
	for (int i = 0; i < 4; ++i)
	{
		sum +=sideArr[i];
	}
	if (sum < 3) return true;
	return false;
}
