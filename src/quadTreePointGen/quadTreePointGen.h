#ifndef _QUADRO_MESH_H_
#define _QUADRO_MESH_H_

#include <usesTypes.h>
#include <node.h>
#include <objectPool.h>
#include <rect.h>
#include <memManager.h>

class QuadroMesh
{
private:
	MemManager * memManager; // менеджер памяти

	//прямоугольник, не пересекающие границу
	NodeBack<Rect *> * rectPureF; //начало списка 
	NodeBack<Rect *> * rectPureE; //конец списка 
	int rectPureCnt;

	//прямоугольник, пересекающие границу
	NodeBack<Rect *> * rectSegF; //начало списка 
	NodeBack<Rect *> * rectSegE; //конец списка 
	int rectSegCnt;

	//ссылка на массив указателей на отрезки границы
	Segment ** border;
	int borderCnt;

	//минимальный размер ребра прямоугольника
	float stopLength;

	void FirstRect(Point ** pointsList, int count);//вычисление параметров для первого прямоугольника

	//вычисление крайних точек границы. результат записывается 
	//в верхнюю левую и нижнюю правую вершина первого прямоугольника 
	void CalcEdges(Point * p1, Point * p3, Point ** pointsList, int count); 

	//заполнение border
	void CalcBorder(Point ** _border, int _borderCount);

	//проверка условия продолжение измельчения сетки
	bool IsCountinue();

	//расщипление прямоугольникa
	void SplitRect(Rect * rect);

	//добавление нового прямоугольникак прямоугольнй сетке
	//в замисимости от пересечения с границей в rectPureE или в rectPureE
	void AddRect(Rect * rect, Rect * root);

	//добавление отрезка в список в прямоугольнике
	void AddSegment(Rect * rect, Segment * seg);

	//добавление соседей для прямоугольников
	void AddNeighbour(Rect * rect1, Rect * rect2);

	//удаление нод для отрезков и соседей внутри прямоугольника
	void FreeNodeInRect(Rect * rect);

	//удаление прямоугольника и ноды для него RemoveRect. переопределение ссылки rectSegCnt
	void RemoveRect();

	//проверка прямоугольника на прнадлежность области
	bool IsRectInRegion(Rect * rect);

	//сглаживание сетки
	void Smoth();

	//проверка прямоугольника на слишком мелких соседей
	bool CheckRect(Rect *  rect);

	
	void BreakNeihbRef(Rect * rect);

	void DropFromNeihb(Rect * fromRect, Rect * target);
public:
	void PrintInfo();
	void Start();
	void GetSegments(Segment ** resSeg, int * count);
	QuadroMesh(MemManager * _memMan, Point ** _border, int _borderCount, float _stopLength);
	~QuadroMesh();
};
#endif
