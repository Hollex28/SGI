/****************************************************************
SGI :: Practica 1
@Author : Adrian Tendero Lara
********************************************************************/
#define PROYECTO "Practica1 SG1"
#include <iostream>
#include <gl/freeglut.h>

using namespace std;
void display()
{
	glClearColor(0.5, 0.0, 1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	}

void reshape(GLint w, GLint h) {
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 200);
	glutCreateWindow(PROYECTO);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	cout << PROYECTO << " en marcha" << endl;
	glutMainLoop();
}