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
double static velocidad = 0; //Modulo de la velocidad en m/s
double static alpha = 0; // Angulo de la velocidad
double static PosX = 0;
double static PosZ = 0;
double static MirarX = 1;
double static MirarZ = 0;
double static last = 0;
const float pi = 3.1415;

bool static inicio = TRUE;
double static NormalX = 0;
double static NormalZ = 0;
float static A = 1;//A=1,5
float static T = 10;//T=20,400 funcionan mas o menos bien

double funcionCarretera(double x) {
	double res = A * sin(x * (2 * pi) / T);
	return res;
}

double DerivadafuncionCarretera(double x) {
	double res = ((2 * pi*A) / T) * cos(x* ((2 * pi) / T));
	return res;
}

void CalculoVNormal(double x) {
	double operador = 0;
	operador = 1 / (sqrt(1 + DerivadafuncionCarretera(x)));
	NormalX = operador * - DerivadafuncionCarretera(x);
	NormalZ = operador;
}
void GirarIzquierda() {
	if (alpha < 1) {
		alpha += 0.01;
	}
}
void GirarDerecha() {
	if (alpha > -1) {
		alpha -= 0.01;
	}
}
void AumentarVelocidad() {
	if (velocidad < 0.2) {
		velocidad += 0.01;
	}
}
void ReducirVelocidad() {
	if (velocidad >=0.01) {
		velocidad -= 0.01;
	}
}
void Calculodeposicioncamara() {
	static int antes = 0;
	int ahora, t;
	ahora = glutGet(GLUT_ELAPSED_TIME);			//Tiempo transcurrido desde el inicio
	t = ahora - antes;
	antes = ahora;
	// V = vo + a * (t-to)
	//e=e0+V*(t-to)+1/2a*(t-to)^2
	PosX = PosX + (velocidad * cos(alpha))*t;
	PosZ = PosZ + (velocidad * -sin(alpha))*t;
	//MirarX = PosX + (abs(velocidad) * cos(alpha));
	//MirarZ = PosZ + (abs(velocidad) * -sin(alpha));
	MirarX = PosX + cos(alpha)*t;
	MirarZ = PosZ - sin(alpha)*t;
	//cout << "sin t " << PosX << " con t " << PosX + (velocidad * cos(alpha)) << endl;
	//cout << "X " << PosX << "Z " << PosZ << "Sin T  X: " << PosX + (velocidad * cos(alpha)) << "Z: " << PosZ + (velocidad * -sin(alpha))*t << "\n";
	glutSwapBuffers();

}
void GeneracionCircuito(int tama�oX) {

	float i;
	double static FinQuak;
	double Zant = funcionCarretera(PosX-1);
	double Zpost;
	double X;
	CalculoVNormal(PosX + (velocidad * cos(alpha)));
	if (inicio) { FinQuak = -10000; }
	for (i = -1; i <= tama�oX; i=i+0.1) {
		X = PosX-1 + (velocidad * cos(alpha)) + i;
		Zpost = funcionCarretera(i + tama�oX);
		GLfloat v0[3] = { X - (NormalX * A / 2),0,Zant - (NormalZ * A / 2) },
			v3[3] = { X + (NormalX * A / 2),0,Zant + (NormalZ * A / 2) };
		/*if (i == 0 && inicio) {
			cout << "Vn(Xn,Yn,Zn): v0 = ( " << X - (NormalX * 5 / 2) << " , 0 , " << Zant - (NormalZ * 5 / 2) << "), v3 =  " << X + (NormalX * 5 / 2) << " , 0, " << Zant + (NormalZ * 5 / 2);
		}*/
		CalculoVNormal(PosX+1);
		GLfloat	v2[3] = { X+1 - (NormalX * A / 2),0,Zant - (NormalZ * A / 2) },
			v1[3] = { X+1 + (NormalX * A / 2),0,Zant + (NormalZ * A / 2) };
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(1, 0.5, 0.5);
		quad(v0, v2, v1, v3, 30, 20);
		Zant = Zpost;
		/*if (i == 0 && inicio) {
			cout << "), v2 = ( " << X+1 - (NormalX * 5 / 2) << " , 0 , " << Zant - (NormalZ * 5 / 2) << "), v3 =  " << X+1 + (NormalX * 5 / 2) << " , 0, " << Zant + (NormalZ * 5 / 2)<< ")\n";
		}*/
	}

	if (i >= FinQuak - 10 || i == -1) {//La condicion 1<=FinQuak-20 el -20 se saca del campo de vision que hay puesto en el reshape
		FinQuak = i + tama�oX;
		glutSwapBuffers();
		//cout << "genero, POSX = " << i << "Fin de Quak = " << FinQuak << "\n";
	}
}

/*void GeneracionCircuito(int tama�oX) {
	double i = PosX;
	double static FinQuak;
	if (inicio) { FinQuak = -10000; }
	double Zant = funcionCarretera(i - 1);
	double Zpost = funcionCarretera(i + tama�oX);
	GLfloat v0[3] = { i - 1,0,Zant+5/2 }, v1[3] = { i + tama�oX,0,Zpost+5/2 },
			v2[3] = { i + tama�oX,0,Zpost-5/2 }, v3[3] = { i - 1,0,Zant-5/2 };
	glPolygonMode(GL_FRONT, GL_LINE);
	glColor3f(1, 0.5, 0.5);
	quad(v0, v3, v2, v1, 30, 20);
	
	if (i>=FinQuak-10 || i == 0) {//La condicion 1<=FinQuak-20 el -20 se saca del campo de vision que hay puesto en el reshape
		FinQuak = i + tama�oX;
		glutSwapBuffers();
		cout << "genero, POSX = " << i << "Fin de Quak = " << FinQuak<<"\n";
	}
}*/
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
	if (inicio){
		gluLookAt(0, 1, 0, 1, 1, 0, 0, 1, 0);
		GeneracionCircuito(20);
	}
	else {
		/*double static PosX = 0;
		double static PosZ = 0;
		double static MirarX = 1;
		double static MirarZ = 0;*/
		gluLookAt(PosX, 1, PosZ, MirarX, 1, MirarZ, 0, 1, 0); // Posiciona la camara
		GeneracionCircuito(20);
	}

		
	
	
}
void onSpecialKey(int tecla, int x, int y)
{
	// Callback de atencion al teclado

	switch (tecla) {
	case GLUT_KEY_UP:
		AumentarVelocidad();
		inicio = false;
		break;
	case GLUT_KEY_DOWN:
		ReducirVelocidad();
		inicio = false;
		break;
	case GLUT_KEY_LEFT:
		GirarIzquierda();
		inicio = false;
		break;
	case GLUT_KEY_RIGHT:
		GirarDerecha();
		inicio = false;
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
	gluPerspective(45, razon, 1, 25);
}
void update()
{
	Calculodeposicioncamara();

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
	// Fase de actualizacion
	Calculodeposicioncamara();


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
	glutSpecialFunc(onSpecialKey);
	glutIdleFunc(onIdle);
	init();
	glutMainLoop();
};