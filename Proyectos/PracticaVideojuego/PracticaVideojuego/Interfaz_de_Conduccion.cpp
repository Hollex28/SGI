/***************************************************
ISGI::Seleccion de objetos
Adtenla
Interaccion
Dependencias:
+GLUT
***************************************************/
#define PROYECTO "Proyecto Videojuego"
#include <iostream>
#include <GL/freeglut.h>
#include <Utilidades.h>

using namespace std;
double static velocidadx,velocidadz = 0;
double static posicionx = 0;
double static posicionz = 0;

void AumentarVelocidadX() {
	if (velocidadx < 1)
		velocidadx += 0.2;
}
void ReducirVelocidadX() {
	if (velocidadx >= 0.2)
		velocidadx -= 0.2;
}
void AumentarVelocidadZ() {
	if (velocidadz < 1.5)
		velocidadz += 0.5;
}
void ReducirVelocidadZ() {
	if (velocidadz > 0.5)
		velocidadz -= 0.5;
}
void init()
//Inicializaciones
{
	// Mensajes por consola
	cout << PROYECTO << " running" << endl;
	cout << "Version: OpenGL " << glGetString(GL_VERSION) << endl;
	glClearColor(1.0, 1.0, 1.0, 1.0); // Color de fondo
};
void onDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posicionx, 1, posicionz, 1, 1, 0, 0, 1, 0); // Posiciona la camara
	//Creacion del circuito
	GLfloat v0[3] = { 0,0,5 }, v1[3] = { 20,0,5 }, v2[3] = { 20,0,-5 }, v3[3] = { 0,0,-5 };
	glPolygonMode(GL_FRONT, GL_LINE);
	glColor3f(0, 1, 0);
	quad(v0, v1, v2, v3, 10, 5);
	glutSwapBuffers();
}
void onSpecialKey(unsigned char tecla, int x, int y)
{
	// Callback de atencion al teclado

	switch (tecla) {
	case GLUT_KEY_UP:
		AumentarVelocidadZ();
		break;
	case GLUT_KEY_DOWN:
		ReducirVelocidadZ();
		break;
	case GLUT_KEY_LEFT:
		ReducirVelocidadX();
		break;
	case GLUT_KEY_RIGHT:
		AumentarVelocidadX();
		break;
	}

	glutPostRedisplay();
}

void onReshape(int w, int h)
// Funcion de atencion al redimensionamiento
{
	// Usamos toda el area de dibujo
	glViewport(0, 0, w, h);
	// Definimos la camara (matriz de proyeccion)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float razon = (float)w / h;
	/* CAMARA PERSPECTIVA */
	gluPerspective(60, razon, 1, 10);
}
void update()
{
	// Fase de actualizacion
	posicionx = posicionx + velocidadx;
	posicionz = posicionz + velocidadz;

	// Mandar evento de redibujo
	glutPostRedisplay();

}
void onTimer(int tiempo)
{
	// CallBack de atencion a la cuenta atras
	update();

	// Una nueva cuenta atras
	glutTimerFunc(tiempo, onTimer, tiempo);

}
void onIdle()
// Funcion de atencion al evento idle
{

}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(PROYECTO);
	glutReshapeFunc(onReshape);
	glutDisplayFunc(onDisplay);
	glutTimerFunc(1000 / 50, onTimer, 1000 / 50);
	glutKeyboardFunc(onSpecialKey);
	glutIdleFunc(onIdle);
	init();
	glutMainLoop();
};