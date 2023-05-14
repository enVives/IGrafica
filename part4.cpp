// Doble Pèndul
// Simulació del doble Pèndul utilitzant les equacions de Lagrange
// Pere Joan Vives Morey i Arnau Vidal Moreno

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

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

float posicio_apunta_x = 0;
float posicio_apunta_y =0;
float posicio_apunta_z =0;

float posicio_camera_x = 5;
float posicio_camera_y =0;
float posicio_camera_z =0;

float angleincrement = 0.0f;
int sentit = 1;

bool seguir = true;




/* Rutina asociada a eventos de ventana ------------------------------------*/
void eventoVentana(GLsizei ancho, GLsizei alto)
{

    glViewport(0, 0, ancho, alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // compara aquí amb ortho i frustrum
	gluPerspective(45.0f,(GLdouble) ancho/alto , 1.0, 20.0);
}

void opcionesVisualizacion(void)
{

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    printf(" flecha superior - enfocar la càmera cap a dalt\n");
    printf(" flecha inferior - enfocar la càmera cap a baix\n");
    printf("flecha esquerre - enfocar la càmera cap a l'esquerre\n");
    printf("flecha dreta - enfocar la càmera cap a la dreta\n");

}
void ProcessSpecialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
    {
        posicio_apunta_x += 0.1;
    }else if(key == GLUT_KEY_RIGHT){
		posicio_apunta_x -= 0.1;
	}else if (key == GLUT_KEY_UP){
		posicio_apunta_y -=0.1;
	}else if (key == GLUT_KEY_DOWN){
		posicio_apunta_y += 0.1;
	}

    glutPostRedisplay();
}

void volta_eixy(){


	double posi_x = posicio_camera_x;
	double posi_z = posicio_camera_z;
	int passes = 0;
	angleincrement = sentit*1*M_PI/180;

	if(angleincrement >= 2*M_PI) {
		angleincrement = 0.0f;
	}
					posicio_camera_x = posi_x* cos(angleincrement) -posi_z* sin(angleincrement);
					posicio_camera_z = posi_x* sin(angleincrement) + posi_z* cos(angleincrement);
					glutPostRedisplay();
					sleep(0.1);

}

void volta_eixx(){


	double posi_x = posicio_camera_x;
	double posi_y = posicio_camera_y;
	int passes = 0;
	angleincrement = sentit*1*M_PI/180;

	if(angleincrement >= 2*M_PI) {
		angleincrement = 0.0f;
	}
					posicio_camera_x = posi_x* cos(angleincrement) +posi_y* sin(angleincrement);
					posicio_camera_y = -posi_x* sin(angleincrement) + posi_y* cos(angleincrement);
					glutPostRedisplay();
					sleep(0.1);

}

void ProcessNormalKeys(unsigned char tecla, int x, int y){

	switch(tecla){
		case 'w':
				if(posicio_camera_z > -5){
					posicio_camera_z -= 0.1;
				}
			break;
		case 's':
				if(posicio_camera_z < 5){
					posicio_camera_z += 0.1;
				}
				break;
		case 'd':
				if(posicio_camera_x < 5){
					posicio_camera_x += 0.1;
				}
				break;
		case 'a':
				if(posicio_camera_x > -5){
					posicio_camera_x -= 0.1;
				}
				break;
		case 'x':
				if(posicio_camera_y < 5){
					posicio_camera_y +=0.1;
				}
				break;
		case 'z':
				if(posicio_camera_y > -5){
					posicio_camera_y -=0.1;
				}
			break;
		case 'r':
				sentit *= -1;
				break;
		case 'e':
				{std::thread t1(volta_eixy);
				t1.join();}
				break;
		case 'f':
				{std::thread t2(volta_eixx);
				t2.join();}
				break;
	}
	glutPostRedisplay();
}


void Display(void)
{

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable (GL_LINE_SMOOTH);
  glEnable( GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(posicio_camera_x, posicio_camera_y, posicio_camera_z, posicio_apunta_x, posicio_apunta_y, posicio_apunta_z, 0.0, 1.0, 0.0);
  

  // 1.25 min per a z
  // 1.5 min per y
  glPushMatrix();
		glScalef(1.5,1.5,1.5);
		glBegin(GL_POLYGON);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(-0.25,-(R1+R2+0.25),-0.25);
            glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(-0.25,-(R1+R2+0.25),0.25);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.25,-(R1+R2+0.25),0.25);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.25,-(R1+R2+0.25),-0.25);
		glEnd();

		glLineWidth (2.0);
		glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-0.25,-(R1+R2+0.25),-0.25);
            glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0,0,-0.25);
		glEnd();

		glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0.25,-(R1+R2+0.25),-0.25);
            glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0,0,-0.25);
		glEnd();

		glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-0.25,-(R1+R2+0.25),0.25);
            glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0,0,0.25);
		glEnd();

		glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0.25,-(R1+R2+0.25),0.25);
            glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0,0,0.25);
		glEnd();

		glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0,0,-0.25);
            glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0,0,0.25);
		glEnd();

		glPushMatrix();
		glTranslatef(-0.25,-(R1+R2+0.25),-0.25);
		glScalef(1.25,1.25,1.25);
		glColor3f(0.0f, 0.0f, 1.0f);
		glutSolidSphere(0.03f,30.0,30.0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.25,-(R1+R2+0.25),0.25);
		glScalef(1.25,1.25,1.25);
		glColor3f(0.0f, 0.0f, 1.0f);
		glutSolidSphere(0.03f,30.0,30.0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.25,-(R1+R2+0.25),0.25);
		glScalef(1.25,1.25,1.25);
		glColor3f(0.0f, 0.0f, 1.0f);
		glutSolidSphere(0.03f,30.0,30.0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.25,-(R1+R2+0.25),-0.25);
		glScalef(1.25,1.25,1.25);
		glColor3f(0.0f, 0.0f, 1.0f);
		glutSolidSphere(0.03f,30.0,30.0);
		glPopMatrix();

		//tripode
		glPushMatrix();
		glTranslatef(0,0,-0.25);
		glScalef(1.25,1.25,1.25);
		glColor3f(0.0f, 0.0f, 1.0f);
		glutSolidSphere(0.03f,30.0,30.0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0,0,0.25);
		glScalef(1.25,1.25,1.25);
		glColor3f(0.0f, 0.0f, 1.0f);
		glutSolidSphere(0.03f,30.0,30.0);
		glPopMatrix();
		
	glPopMatrix();

  glLineWidth (5.0);

	glPushMatrix();
		glScalef(1.5,1.5,1.5);
		glRotatef((GLfloat) angle1*180/M_PI,0.0f,0.0f,1.0f);

		glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f,0.0f);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f((GLfloat)xx1,(GLfloat)yy1,0.0f);	
		glEnd();

		glLineWidth (1.0);
		glColor3f(1.0f, 0.0f, 1.0f);
        glutSolidSphere(0.03f,30.0,30.0);
		

        glTranslatef(xx1,yy1,0.0f);


		glColor3f(0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.03f,30.0,30.0);

	glPopMatrix();

	glLineWidth (5.0);

	glPushMatrix();
		glScalef(1.5,1.5,1.5);
		glRotatef((GLfloat) angle1*180/M_PI,0.0f,0.0f,1.0f);
		glTranslatef(0.0f,(GLfloat)-R1,0.0f);
		glRotatef((GLfloat) angle2*180/M_PI, 0.0f, 0.0f, 1.0f);

		glBegin(GL_LINES);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f((GLfloat)0.0f, (GLfloat)0.0f,(GLfloat)0.0f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f((GLfloat)xx2,(GLfloat)yy2,(GLfloat)0.0f);
		glEnd();

		glLineWidth (1.0);

        glTranslatef(xx2,yy2,0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.03f,30.0,30.0);

	glPopMatrix();


	glutSwapBuffers();
	glFlush();
}

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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutCreateWindow("Pendulo");

	glutDisplayFunc(Display);
	glutIdleFunc(lagrange);
	glutSpecialFunc(ProcessSpecialKeys);
    glutReshapeFunc(eventoVentana);
	glutKeyboardFunc(ProcessNormalKeys);

	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f); // minims i màxims x,y i z que veim

    opcionesVisualizacion();

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}