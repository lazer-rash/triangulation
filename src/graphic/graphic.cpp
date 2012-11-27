#include "graphic.h"
#include "canvas.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <iostream>
Canvas *  currentCanv;

void SetupRC();
void InitOpenGL();
void RenderScene();
void ReadSegGL();

void InitOpenGL()
{
	int argc = 0;
	char *argv[argc];
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowPosition(50,25);
	glutInitWindowSize(500,500);
	glutCreateWindow("triangulation");
	SetupRC();
	glutDisplayFunc(RenderScene);
	glutMainLoop();
}

void SetupRC()
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(currentCanv->leftBottom.x, currentCanv->rightTop.x, currentCanv->leftBottom.y, currentCanv->rightTop.y, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1, 1, 1, 0);
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
	ReadSegGL();
	//glPopMatrix();
	//glPushMatrix();
	glFlush();
}

void ReadSegGL()
{
	glBegin(GL_LINES);
	{
		for (int i = 0; i < currentCanv->segCnt; ++i)
		{
			glVertex2f(currentCanv->segToPlot[i].vertex[0]->x, currentCanv->segToPlot[i].vertex[0]->y);
			glVertex2f(currentCanv->segToPlot[i].vertex[1]->x, currentCanv->segToPlot[i].vertex[1]->y);
		}
	}
	glEnd();

}

void Plot(Segment * segToPlot,int  segCnt)
{
	cout<<"Plot"<<endl;
	Canvas canvas(segToPlot, segCnt);
	currentCanv = &canvas;
	InitOpenGL();
}