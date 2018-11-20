/**
*  Animacion.cpp
*
*  Ejemplos de animacion en OpenGL
*
*  @author:   rvivo@upv.es
*  @version:  v1.0 Nov,2018
*  @require:  freeglut, utilidades
*
**/

#define PROYECTO "ISGI::S5E01::Animacion"

#include <iostream>
#include <sstream>
#include <Utilidades.h>
#include <ctime>

using namespace std;

// Variables dependientes del tiempo
static float segundo;
static float minuto;
static float hora;
static float alpha;


void init()
{
	// Configurar el motor de render
	glClearColor(0, 0, 0.3, 1);
	glEnable(GL_DEPTH_TEST);

}

void FPS()
{
	// Cuenta fotogramas hasta que pasa un segundo
	// Entonces los muestra y reinicia la cuenta

	int ahora, tiempo_transcurrido;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	static int fotogramas = 0;

	// Cada vez que paso sumo un fotograma
	fotogramas++;

	// Calular el tiempo 
	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempo_transcurrido = ahora - antes;

	// si ha transcurrido mas de un segundo muestro y reinicio
	if (tiempo_transcurrido > 1000) {
		stringstream titulo;
		titulo << "FPS = " << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());
		fotogramas = 0;
		antes = ahora;
	}
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Seleccionar la MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situar y orientar la camara: punto de vista, punto de interes, vertical
	gluLookAt(0, 0, 7, 0, 0, 0, 0, 1, 0);

	//ejes();
	static GLuint horas, Marcadores;
	// Marcador de hora
	horas = glGenLists(1);
	glNewList(horas, GL_COMPILE);

	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glScalef(0.15, 0.25, 0.15);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();
	glEndList();


	// Marcadores de hora
	Marcadores = glGenLists(1);
	glNewList(Marcadores, GL_COMPILE);

	for (auto i = 0; i < 12; i++) {
		
		glPushMatrix();
		glRotatef(i * 30, 0, 0, 1);
		glTranslatef(00, -1, 0.0);
		glScalef(0.25, 0.25, 0.25);
		glRotatef(alpha, 0, 1, 0);
		glutSolidTeapot(.5);
		glPopMatrix();
	}

	glPopMatrix();

	glEndList();

	
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor3f(0, 1, 0);
	glRotatef(0, 0, 1, 0);
	glCallList(Marcadores);
	glColor3f(1, 1, 1);
	glPopMatrix();
	//Segundero
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor3f(1, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glRotatef(segundo * 6, 0, 1, 0);
	glutSolidCone(0.05, 0.70, 10, 10);
	glPopMatrix();
	// Minutero
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor3f(1, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glRotatef(minuto * 6, 0, 1, 0);
	glutSolidCone(0.05, 0.60, 10, 10);
	glPopMatrix();
	//Horero
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor3f(1, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glRotatef(hora * 30, 0, 1, 0);
	glutSolidCone(0.05, 0.40, 10, 10);
	glPopMatrix();
	//centro
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.05, 20, 20);

	glutSwapBuffers();

	
	FPS();
}

void reshape(GLint w, GLint h)
{
	// Toda el area como marco
	glViewport(0, 0, w, h);

	// Razon de aspecto
	float ra = float(w) / float(h);

	// Matriz de la proyeccion

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Camara ortografica con isometria
	//if(ra>1) glOrtho(-2*ra, 2*ra, -2, 2, -2, 2);
	//else glOrtho(-2, 2, -2 / ra, 2 / ra, -2, 2);

	// Camara perspectiva
	gluPerspective(20, ra, 0.2, 10);
}
#pragma warning(disable : 4996)
void update()
{
	// Fase de actualizacion
// Conseguir hora
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	//sacar tiempos
	segundo = timeinfo->tm_sec;
	minuto = timeinfo->tm_min;
	hora = timeinfo->tm_hour;
	alpha++;
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
void main(int argc, char** argv)
{
	// Initializations
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutIdleFunc(update);
	glutTimerFunc(1000 / 50, onTimer, 1000 / 50);

	cout << PROYECTO << " en marcha" << endl;

	// Event loop
	glutMainLoop();
}