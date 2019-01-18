#include <iostream> // Biblioteca de entrada salida
#include <cmath> // Biblioteca matemática de C
#include <gl\freeglut.h> // Biblioteca grafica
#include <Utilidades.h>
#include <sstream>
#include <ctime> 

#define PROYECTO "Fzero"
#define PI 3.1415926
#define tasaFPS 60

static float ticks = 0;
static const float tickRate = 1;

static enum { SOLID, WIRE } RENDER_MODE;
static enum { DAY, NIGHT } TIME_OF_DAY;
static enum { CLEAR, FOGGY } WEATHER;
static enum { SPEED_OFF, SPEED_ON } UI;
static int lightingPoles[] = { GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6 };

static float displacement = 0.0;
static float speed = 0, despX = 0, despZ = 1;
static float PosX = 0, PosY = 1, PosZ = 0;
static float LastZ = 0;

static float angulo = 7.5;

static GLuint coche, track, skybox_texture, bannerCheckboard, terreno, grid, spaceship;

time_t h, m, s;

float static A = 10;//A=1,5
float static T = 200;//T=20,400 funcionan mas o menos bien
double static below = -0.001; // acercar un below mas ha 0 causa colisiones extrañas entre la carretera y el suelo
static float coef[16];


// Vertices
static GLfloat v0[3] = { 2,0,30 }, v1[3] = { -2,0,30 }, v2[3] = { -2,0,-50 }, v3[3] = { 2,0,-30 };
static GLfloat v4[3] = { 8,0,-35 }, v5[3] = { 6,0,-50 }, v6[3] = { 26,0,-35 }, v7[3] = { 28,0,-55 };
static GLfloat v8[3] = { 55,0,-32 }, v9[3] = { 44,0,-30 }, v10[3] = { 40,0,-10 }, v11[3] = { 55,0,-12 };
static GLfloat v12[3] = { 40,0,0 }, v13[3] = { 55,0,-2 }, v14[3] = { 42,0,32 }, v15[3] = { 38,0,30 };
static GLfloat v16[3] = { 36,0,32 }, v17[3] = { 38,0,36 }, v18[3] = { 2,0,36 }, v19[3] = { 4,0,32 };

float trazado(float x, float amplitud, float periodo)
{
	return amplitud * sin(x * ((2 * PI) / periodo));
}

void lighting() {

	//Lunar Light
	GLfloat Al0[] = { 0.05,0.05,0.05 };
	GLfloat Dl0[] = { 0.05,0.05,0.05 };
	GLfloat Sl0[] = { 0.0,0.0,0.0 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Al0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Sl0);


	//Car Lighting

	GLfloat Al1[] = { 0.2,0.2,0.2,1.0 };
	GLfloat Dl1[] = { 1.0,1.0,1.0,1.0 };
	GLfloat Sl1[] = { 0.3,0.3,0.3,1.0 };


	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, Al1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Dl1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Sl1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);

	//Lighting Poles
	GLfloat Alx[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat Dlx[] = { 0.5, 0.5, 0.2, 1.0 };
	GLfloat Slx[] = { 0.3, 0.3, 0.3, 1.0 };

	for (int i : lightingPoles) {
		glEnable(i);
		glLightfv(i, GL_AMBIENT, Alx);
		glLightfv(i, GL_DIFFUSE, Dlx);
		glLightfv(i, GL_SPECULAR, Slx);
		glLightf(i, GL_SPOT_CUTOFF, 45.0);
		glLightf(i, GL_SPOT_EXPONENT, 10.0);
	}

	// Material
	GLfloat Dmx[] = { 0.8,0.8,0.8,1.0 };
	GLfloat Smx[] = { 0.3,0.3,0.3,1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Dmx);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Smx);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 3);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
}


void loadtextures() {

	glGenTextures(1, &coche);
	glBindTexture(GL_TEXTURE_2D, coche);
	loadImageFile((char*)"transcaptain falcon.png");
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &track);
	glBindTexture(GL_TEXTURE_2D, track);
	loadImageFile((char*)"carretera.jpg");

	glGenTextures(1, &skybox_texture);
	glBindTexture(GL_TEXTURE_2D, skybox_texture);
	loadImageFile((char*)"skybox.png");

	glGenTextures(1, &terreno);
	glBindTexture(GL_TEXTURE_2D, terreno);
	loadImageFile((char*)"Arena2.jpg");

	glGenTextures(1, &grid);
	glBindTexture(GL_TEXTURE_2D, grid);
	loadImageFile((char*)"grid.jpg");

	glGenTextures(1, &bannerCheckboard);
	glBindTexture(GL_TEXTURE_2D, bannerCheckboard);
	loadImageFile((char*)"checkerBanner.jpg");

	glGenTextures(1, &spaceship);
	glBindTexture(GL_TEXTURE_2D, spaceship);
	loadImageFile((char*)"space ship.jpg");

}

void init()
// Funcion de inicializacion propia
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);

	loadtextures();
	lighting();

	cout << "1. Flecha izquierda/derecha: giro del vehículo\n";
	cout << "2. Flecha arriba / abajo: aumento / disminución de la velocidad\n";
	cout << "3. s : Activa / desactiva un modelo simple en alámbrico de la práctica 6 sin luces ni texturas \n";
	cout << "4. l : Cambia entre modo diurno / nocturno\n";
	cout << "5. n : Cambia el estado de la niebla(on / off)\n";
	cout << "6. c : Cambia la visibilidad de elementos solidarios a la cámara(on / off):\n";

}

void lightingPole(float x, float y, float z,GLuint light) {
	glPushMatrix();
	GLfloat posicionL2[] = { x,y,z,1 };//farola1
	glLightfv(light, GL_POSITION, posicionL2);
	glPopMatrix();
}

void backgroundObjectParams(GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	if (TIME_OF_DAY == DAY) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
	else { glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); }
}

void GeneracionCircuito(int tamaño) {
	backgroundObjectParams(track);
	for (auto i = -(PosZ + 1); i < PosZ + tamaño; i++)
	{
		//GLfloat v0[3] = { trazado(i, A, T) - 2,0,i }, v3[3] = { trazado(i, A, T) + 2,0,i }, v2[3] = { trazado(i + 1, A, T) + 2,0, i + 1 }, v1[3] = { trazado(i + 1, A, T) - 2,0, i + 1 };
		GLfloat v1[3] = { trazado(i, A, T) - 2,0,i }, 
			v2[3] = { trazado(i, A, T) + 2,0,i }, 
			v3[3] = { trazado(i + 1, A, T) + 2,0, i + 1 }, 
			v0[3] = { trazado(i + 1, A, T) - 2,0, i + 1 };
		glPushMatrix();
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(1, 0.5, 0.5);

		quad(v3, v0, v1, v2, 30, 30);
		glPopMatrix();
	}

}

void banner(float x, float y, float z, float angle, float base, float height, GLuint texture) {

	glPushMatrix();
	glTranslatef(x, y, z);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	if (TIME_OF_DAY == DAY) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
	else { glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); }
	if (TIME_OF_DAY == DAY) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
	else { glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); }
	
	glRotatef(angle, 0, 1, 0);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-base, -height, 0);
	glTexCoord2f(1, 0);
	glVertex3f(base, -height, 0);
	glTexCoord2f(1, 1);
	glVertex3f(base,height, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-base,height, 0);
	glEnd();

	glPopMatrix();

}

void skybox() {

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, skybox_texture);
	backgroundObjectParams(skybox_texture);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 10; i++) {
		float angle = i * 2 * PI / 10;
		glTexCoord2f(i * 1.0 / 10, 0);
		glVertex3f(220 * cos(angle), 200 * -0.5, 220 * sin(angle));
		glTexCoord2f(i * 1.0 / 10, 1);
		glVertex3f(220 * cos(angle), 200 *  0.7, 220 * sin(angle));
	}
	glEnd();
	glPopMatrix();

}

void floor(int Area2,int numero) {
	for (int i = 0; i <= numero; i++) {
		int incerteza = 10;
		glPushMatrix();
		backgroundObjectParams(terreno);
		glBegin(GL_QUADS);
		/*glTexCoord2f(0, 1);
		glVertex3f(PosZ-Area2, below, (PosZ-2)+(Area2*i)); //4
		glTexCoord2f(1, 1);
		glVertex3f(PosZ+Area2, below, (PosZ - 2) + (Area2*i));  //3
		glTexCoord2f(1, 0);
		glVertex3f(PosZ+Area2, below, (PosZ - Area2) + (Area2*i)); //2
		glTexCoord2f(0, 0);
		glVertex3f(PosZ-Area2, below, (PosZ - Area2) + (Area2*i));//1
		*/
		float aux = (PosZ / Area2); //Saco la parte decimal de la cantidad de carretera recorrida de la parte generada
		int aux1 = (int)aux;
		aux = aux - aux1;
		if ( aux >0.99|| PosZ == 0) {
			glTexCoord2f(0, 0);
			glVertex3f(-Area2, below, (PosZ - Area2) + (Area2*i));//1
			glTexCoord2f(1, 0);
			glVertex3f(Area2, below, (PosZ - Area2) + (Area2*i)); //2
			glTexCoord2f(1, 1);
			glVertex3f(Area2, below, (PosZ)+((Area2)*i));  //3
			glTexCoord2f(0, 1);
			glVertex3f(-Area2, below, (PosZ)+((Area2)*i)); //4
			LastZ = PosZ;

		}else{
			glTexCoord2f(0, 0);
			glVertex3f(-Area2, below, (LastZ - Area2) + (Area2*i));//1
			glTexCoord2f(1, 0);
			glVertex3f(Area2, below, (LastZ - Area2) + (Area2*i)); //2
			glTexCoord2f(1, 1);
			glVertex3f(Area2, below, (LastZ)+((Area2)*i));  //3
			glTexCoord2f(0, 1);
			glVertex3f(-Area2, below, (LastZ)+((Area2)*i)); //4
		}


		glEnd();
		glPopMatrix();
	}
}


void space_ship(float x, float y, float z, float angle, float base, float height, float slices, float stacks, GLuint texture) {

	glPushMatrix();
	glTranslatef(x, y, z);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (TIME_OF_DAY == DAY) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
	else { glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); }
	glRotatef(angle, 0, 1, 0);
	if (RENDER_MODE == SOLID) {
		glutSolidCone(base, height, slices, stacks);
	}
	else { glutWireCone(base, height, slices, stacks); }
	glPopMatrix();


}

void car() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	backgroundObjectParams(coche);	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5, 0.1, -2);
	glTexCoord2f(1, 0);
	glVertex3f(0.5, 0.1, -2);
	glTexCoord2f(1, 1);
	glVertex3f(0.5, 0.5, -2);
	glTexCoord2f(0, 1);
	glVertex3f(-0.5, 0.5, -2);
	glEnd();
	glPopMatrix();

}

void speedBar() {

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);

	glBegin(GL_POLYGON);

	if (speed >= 3.0) {
		glColor3f(0.0, 0.9, 0.0);
	}
	if (speed >= 5.5) {
		glColor3f(0.9, 0.9, 0.0);
	}
	if (speed >= 7) {
		glColor3f(1.0, 0, 0.0);
	}

	glTranslatef(-0.5, -5, 0);
	glVertex3f(1.4, 0.8, -2.0);
	glVertex3f(1.4, 0.75, -2.0);
	
	if (speed >= 3.0) {
		glColor3f(1.0, 0.5, 0.0);
	}
	if (speed >= 5.5) {
		glColor3f(1.0, 0.0, 0.0);
	}
	if (speed >= 7) {
		glColor3f(0.6, 0, 0);
	}
	glVertex3f(1.4 - (1.4 - 0.5)*speed / (15 + 0.0001), 0.75, -2.0);
	if (speed >= 10) {
		glColor3f(0, 0, 0.9);
	}
	glVertex3f(1.4 - (1.4 - 0.5)*speed / (15 + 0.0001), 0.8, -2.0);
	glEnd();
	
	glColor3f(1, 1, 1);
	glPopMatrix();
}



void populate_world() {

	lightingPole(0, 0, 0,GL_LIGHT2);
	lightingPole(v4[0], v4[1], v4[2]+1, GL_LIGHT3);
	lightingPole(v1[0], v1[1], v1[2]+1, GL_LIGHT4);
	lightingPole(v2[0], v2[1], v2[2]+1, GL_LIGHT5);
	int area = 20; //area de un segmento de carretera
	int Nseg = 5;
	int incerteza = 10;
	GeneracionCircuito(70);
	floor(area, Nseg);//tamaño de 1 segmento de carretera indivisual + numero de segmentos generados
	//space_ship(-10, 0, -25, 0, 2, 10, 4, 2, spaceship);
	//space_ship(-10, 0, 25, 189, 2, 10, 4, 2, spaceship);
	//space_ship(50, 0, 70, 90, 2, 10, 5, 2, spaceship);

	for (int i = -10; i > -30; i -= 4) {
		banner(-5, 2, i, 0, 0.6, 2, bannerCheckboard);
	}

	//skybox();

	
}

void speed_bar_setting() {
	if (UI == SPEED_ON) {
		glClearColor(1, 1, 1, 1);
		speedBar();
		//mapUI();
	}
}

void time_of_day_setting() {
	if (TIME_OF_DAY == NIGHT) {
		//glClearColor(0, 0, 0, 1);

		glPushMatrix();
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);

		glPopMatrix();
	}
	else {
		glClearColor(1, 1, 1, 1);
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glPopMatrix();
	}

}

void render_mode_setting() {
	if (RENDER_MODE == WIRE) {
		glPushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_TEXTURE_2D);
	}
}

void weather_setting() {
	if (WEATHER == FOGGY) {
		glPushMatrix();
		glEnable(GL_FOG);
		GLfloat colour[] = { 0.6,0.3,0.1 };
		glFogfv(GL_FOG_COLOR, colour);
		glFogf(GL_FOG_DENSITY, 0.2);
		glPopMatrix();
	}
	else {
		glDisable(GL_FOG);
	}
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glNormal3f(0, 1, 0);
	glColor3f(0, 0, 0);

	//HeadLight
	GLfloat posicionL1[] = { 0 ,0.7,0,1 };
	glLightfv(GL_LIGHT1, GL_POSITION, posicionL1);
	GLfloat dir_light1[] = { 0.0, -0.4, -0.7 };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_light1);

	speed_bar_setting();
	glPushMatrix();
	glTranslatef(0, -1, 0);
	car();
	glPopMatrix();

	gluLookAt(PosX, 1, PosZ, PosX + despX, 1, PosZ + despZ, 0, 1, 0);
	//cout << "(" << PosX << "," << PosY << "," << PosZ << "," << PosX << "+" << despX << ",1," << PosZ << "+" << despZ << ")\n";

	time_of_day_setting();
	render_mode_setting();
	weather_setting();


	populate_world();

	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float razon = (float)w / h;
	gluPerspective(45, razon, 1, 100);
}

void logic()
{
	static int init_time = glutGet(GLUT_ELAPSED_TIME);
	
	// Tiempo ahora (Final de la ventana)
	int now = glutGet(GLUT_ELAPSED_TIME);

	// Tiempo transcurrido en la ventana
	float time_diff = (now - init_time) / 1000.0f;

	// Numero de ticks transcurridos desde el inicio de la aplicacion
	ticks += tickRate * 360 * time_diff;

	// Calculo de movmiento
	displacement = time_diff * speed;

	// Movemos la ventana de tiempo
	init_time = now;

	PosX += displacement * despX;
	PosZ += displacement * despZ;

	glutPostRedisplay();
}

void onTimer(int tiempo)
{
	// Sirve para re-arrancar el reloj de cuenta atras
	glutTimerFunc(tiempo, onTimer, tiempo);
	logic();
}

void onArrow(int tecla, int x, int y)
{
	switch (tecla) {
	case GLUT_KEY_UP:
		cout << PosX << " X,  " << PosZ << " Z \n";
		if (speed < 15) speed += 0.5;
		break;
	case GLUT_KEY_DOWN:
		cout << PosX << " X,  " << PosZ << " Z \n";
		if (speed > 0.1) speed -= 1;
		else speed = 0.0;
		break;
	case GLUT_KEY_LEFT:
		cout << angulo << " Angulo";
		angulo += 0.75;
		break;
	case GLUT_KEY_RIGHT:
		angulo -= 0.75;
		cout << angulo << " Angulo";
		break;
	}
	
	// Partimos la distancia en sus componentes basado en el angulo
	despX = sin(angulo*PI / 180);
	despZ = cos(angulo*PI / 180);

	glutPostRedisplay();
}

void onKey(unsigned char key, int x, int y)
{
	switch (key) {
	case 's':
		if (RENDER_MODE == WIRE) { RENDER_MODE = SOLID; }
		else RENDER_MODE = WIRE;
		break;

	case 'l':
		if (TIME_OF_DAY == NIGHT) { TIME_OF_DAY = DAY; }
		else TIME_OF_DAY = NIGHT;
		break;

	case 'n':
		if (WEATHER == FOGGY) { WEATHER = CLEAR; }
		else WEATHER = FOGGY;
		break;

	case 'c':
		if (UI == SPEED_ON) { UI = SPEED_OFF; }
		else UI = SPEED_ON;
		break;
	case 27:
		exit(0);
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
// Programa principal
{
	FreeImage_Initialise();
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Alta de buffers a usar
	glutInitWindowSize(1280, 720); // Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO); // Creacion de la ventana con su titulo
	init(); // Inicializacion propia. IMPORTANTE SITUAR AQUI
	std::cout << PROYECTO << " running" << std::endl; // Mensaje por consola

	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	
	glutTimerFunc(1000 / tasaFPS, onTimer, 1000 / tasaFPS);

	glutSpecialFunc(onArrow);
	glutKeyboardFunc(onKey);

	glutMainLoop(); // Puesta en marcha del programa
	FreeImage_DeInitialise();
}
