// Doble Pèndul
// Simulació del doble Pèndul utilitzant les equacions de Lagrange
// Pere Joan Vives Morey i Arnau Vidal Moreno
//Código para ejecutar : g++ part2.cpp -o part2 -lGL -lGLU -lglut

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>

const int W_WIDTH = 500; // Tama�o incial de la ventana
const int W_HEIGHT = 500;


const float R1 = 0.5f;
const float R2 = 0.3f;
const float g = 9.8f;
const float M1 = 0.1f;
const float M2 = 0.2f;

float angle1 = 128*M_PI/180;
float angle1dot = 0;
float angle2 = 35*M_PI/180;
float angle2dot =0;
float t = 0;
float dt = 0.01;
float xx1 = 0.0f;
float yy1 = -R1;

float xx2 = 0.0f;
float yy2 = -R2;

//función que nos dibuja un círculo dados una posición, un radio y un número de segmentos.
void drawCircle(float cx, float cy, float r, int num_segments)
{
    float theta = 3.1415926 * 2 / float(num_segments);
    float tangetial_factor = tanf(theta);

    float radial_factor = cosf(theta);

    float xx = r;

    float yy = 0;
    glLineWidth(2);
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++)
    {
        glVertex2f(xx + cx, yy + cy);


        float tx = -yy;
        float ty = xx;

        xx += tx * tangetial_factor;
        yy += ty * tangetial_factor;


        xx *= radial_factor;
        yy *= radial_factor;
    }
    glEnd();
}

void Display(void)
{

  glClear(GL_COLOR_BUFFER_BIT);
  glLineWidth (5.0);

	//en este primer push matrix vamos a pintar el primer brazo del péndulo
	//vamos a rotarlo con el angulo1
	//luego pintamos dos círculos, que representan los puntos de unión 
	//del principio del péndulo i del segundo brazo

	glPushMatrix();

		glRotatef((GLfloat) angle1*180/M_PI,0.0f,0.0f,1.0f);

		//primer brazo
		glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(0.0f, 0.0f);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f((GLfloat)xx1,(GLfloat)yy1);	
		glEnd();

		//primer círculo, este unirá el primer brazo con el segundo
		glPushMatrix();
			glTranslatef(xx1,yy1,0.0f);
			glBegin(GL_POLYGON);
				glColor3f(1.0f, 0.0f, 1.0f);
				drawCircle(0.0f,0.0f,0.03f,30);
			glEnd();
		glPopMatrix();

		//segundo círculo, este estará al inicio
		glBegin(GL_POLYGON);
		glColor3f(0.0f, 0.0f, 0.0f);
			drawCircle(0.0f,0.0f,0.03f,30);
		glEnd();

	glPopMatrix();

	glLineWidth (5.0);

	//en este pushMatrix vamos a pintar el segundo brazo i el círculo que está al final del péndulo
	//el orden de las transformaciones va a ser el siguiente
	//1) giramos el brazo con su ángulo (angle2)
	//2) lo movemos donde el primer brazo terminaba al inicio
	//3) lo volvemos a girar pero ahora con el angulo del primer brazo.
	glPushMatrix();

		glRotatef((GLfloat) angle1*180/M_PI,0.0f,0.0f,1.0f);
		glTranslatef(0.0f,(GLfloat)-R1,0.0f);
		glRotatef((GLfloat) angle2*180/M_PI, 0.0f, 0.0f, 1.0f);

		glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f((GLfloat)0.0f, (GLfloat)0.0f);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f((GLfloat)xx2,(GLfloat)yy2);
		glEnd();

		glLineWidth (1.0);
		glTranslatef(xx2,yy2,0.0f);
		glBegin(GL_POLYGON);
		glColor3f(1.0f, 0.0f, 1.0f);
			drawCircle(0.0f,0.0f,0.03f,30);
		glEnd();
		
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
}

//Función donde, utilizando las funciones de lagrange sacamos los angulos que van a tenerç
//los brazos del péndulo
void lagrange(void){

	float a = (-(M1+M2) *g*R1 * sin(angle1)) - (M2*R1*R2*pow(angle2dot,2)*sin(angle1-angle2));
	float b = (M1+M2) *pow(R1,2);
	float c = M1*R1*R2*cos(angle1-angle2);
	float f = (-M2*g*R2*sin(angle2))+(M2*R1*R2*pow(angle1dot,2)*sin(angle1-angle2));
	float k = M2*pow(R2,2);
	float w = M2*R1*R2*cos(angle1-angle2);

	float angle2dotdot = (f-a*w/b)/(k-c*w/b);
	float angle1dotdot = a/b - c*angle2dotdot/b;
	angle2dot = angle2dot+ angle2dotdot*dt;
	angle1dot = angle1dot + angle1dotdot*dt;
	angle2 = angle2+ angle2dot*dt;
	angle1 = angle1+ angle1dot*dt;
	t = t+dt;

	glutPostRedisplay();
}

//Función que modifica el volumen de visualización para que tenga las mismas 
//proporciones que la ventana y de esta manera se evita la distorsión.
void Reproyectar(int width, int height){

	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat ratio;

	if(width>height){
		ratio = (GLfloat) width/ (GLfloat) height;
		glOrtho(-1.0 * ratio, 1.0f*ratio, -1.0, 1.0f, -1.0, 1.0f);
	}else{
		ratio = (GLfloat) height/ (GLfloat) width;
		glOrtho(-1.0, 1.0f, -1.0*ratio, 1.0f*ratio, -1.0, 1.0f);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutCreateWindow("Pendulo");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reproyectar);
	glutIdleFunc(lagrange);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f); // minims i màxims x,y i z que veim

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}