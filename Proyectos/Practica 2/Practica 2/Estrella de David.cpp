/***************************************************
ISGI::Pentagono coloreado
Roberto Vivo', 2013 (v1.0)
Estrella de David
Dependencias:
+GLUT
***************************************************/
#define PROYECTO "ISGI::S2E03::Pentagono Colores"
#include <iostream> // Biblioteca de entrada salida
#include <cmath> // Biblioteca matemática de C
#include <gl\freeglut.h> // Biblioteca grafica
GLuint starofdavid;
void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT); // Borra la pantalla
	// Dibujo como POLYGON con los vertices coloreados
	glClearColor(1.0,1.0,1.0,0.0);
	float pi = 3.1415926;
	starofdavid = glGenLists(1);
	glNewList(starofdavid, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.0, 0.0, 0.3);
	for (int i = 0; i <= 3; i++) {
		glVertex3f(0.7 * cos(i*((2 * pi) / 3) + pi / 2), 0.7 * sin(i*((2 * pi) / 3) + pi / 2),0);
		glVertex3f(1 * cos(i*((2 * pi) / 3) + pi / 2), 1 * sin(i*((2 * pi) / 3) + pi / 2), 0);
	}
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.0, 0.0, 0.3);
	for (int i = 0; i <= 3; i++) {
		glVertex3f(0.7 * cos(i*((2 * pi) / 3) + pi / 2), 0.7 * sin(i*((2 * pi) / 3) - pi / 2), 0);
		glVertex3f(1 * cos(i*((2 * pi) / 3) + pi / 2), 1 * sin(i*((2 * pi) / 3) - pi / 2), 0);
	}
	glEnd();
	glEndList();
	glCallList(starofdavid);
	
	glFlush(); // Finaliza el dibujo
}
void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
}
void main(int argc, char** argv)
// Programa principal
{
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Alta de buffers a usar
	glutInitWindowSize(400, 400); // Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO); // Creacion de la ventana con su titulo
	std::cout << PROYECTO << " running" << std::endl; // Mensaje por consola
	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	glutMainLoop();
}