// Doble Pèndul
// Simulació del doble Pèndul utilitzant les equacions de Lagrange
// Pere Joan Vives Morey i Arnau Vidal Moreno
//Compilar: g++ part4.cpp -o part4 -lGL -lGLU -lglut -pthread

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

float posicio_camera_x = 0;
float posicio_camera_y =0;
float posicio_camera_z =5;

float verticalx = 0;
float verticaly = 1;
float verticalz = 0;

float angleincrement = 0.0f;

float anglexz = 0.0;
float anglexy = 0.0;
int direccio =1;
int sentit = 1;

const float limit = 0.000001;

bool seguir = true;

int posicio =3;



/* Rutina asociada a eventos de ventana ------------------------------------*/
void eventoVentana(GLsizei ancho, GLsizei alto)
{

    glViewport(0, 0, ancho, alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // compara aquí amb ortho i frustrum
	gluPerspective(45.0f,(GLdouble) ancho/alto , 0.0, 20.0);
}

void gira_vista_y(float angle){
    float vista_x = posicio_apunta_x;
    float vista_z = posicio_apunta_z;
    //float angle = direccio*1*M_PI/180;

    posicio_apunta_x = vista_x*cos(angle) -vista_z*sin(angle);
    posicio_apunta_z = vista_x*sin(angle) +vista_z*cos(angle);
    anglexz += angle;
    
    if(anglexz >=0){
        if(anglexz >= 360*M_PI/180){
            anglexz = 360*M_PI/180 -anglexz;
        }
    }else{
        if(anglexz <= -1*360*M_PI/180){
            anglexz = anglexz +360*M_PI/180;
        }
    }
    glutPostRedisplay();
}

void gira_vista_x(float angle){
    float vista_z = posicio_apunta_z;
    float vista_y = posicio_apunta_y;
    //float angle = direccio*1*M_PI/180;

    posicio_apunta_y = vista_y* cos(angle) -vista_z* sin(angle);
	posicio_apunta_z = vista_y* sin(angle) + vista_z* cos(angle);
    anglexy += angle;
    

    if(anglexy >=0){
        if(anglexy >= 360*M_PI/180){
            anglexy = 360*M_PI/180 -anglexy;
        }
    }else{
        if(anglexy <= -1*360*M_PI/180){
            anglexy = anglexy +360*M_PI/180;
        }
    }
    glutPostRedisplay();
}

void trasladar_inici(){
    posicio_apunta_x -= posicio_camera_x;
    posicio_apunta_y -= posicio_camera_y;
    posicio_apunta_z -= posicio_camera_z;
}

void tornar_lloc(){
    posicio_apunta_x += posicio_camera_x;
    posicio_apunta_y += posicio_camera_y;
    posicio_apunta_z += posicio_camera_z;
}

void girar_inici_xz(){
        gira_vista_y(-anglexz);
}


void ProcessSpecialKeys(int key, int x, int y)
{
	if(posicio == 3){
		if (key == GLUT_KEY_LEFT)
		{
			direccio =-1;
			trasladar_inici();
			gira_vista_y(direccio*1*M_PI/180);
			tornar_lloc();
		}else if(key == GLUT_KEY_RIGHT){
			direccio = 1;
			trasladar_inici();
			gira_vista_y(direccio*1*M_PI/180);
			tornar_lloc();
		}else if (key == GLUT_KEY_UP){
			direccio = 1;
			trasladar_inici();
			float anglexz_copia = anglexz;
			girar_inici_xz();
			gira_vista_x(direccio*1*M_PI/180);
			gira_vista_y(anglexz_copia);
			tornar_lloc();
		}else if (key == GLUT_KEY_DOWN){
			direccio = -1;
			trasladar_inici();
			float anglexz_copia = anglexz;
			girar_inici_xz();
			gira_vista_x(direccio*1*M_PI/180);
			gira_vista_y(anglexz_copia);
			tornar_lloc();
		}	
	}
}


void moure_camera(int cas){
    float angle;
    trasladar_inici();

    if(posicio_apunta_z<0){
        if(posicio_apunta_x>0){
            angle = atan(posicio_apunta_z/posicio_apunta_x) +360.0*M_PI/180;
        }else if(posicio_apunta_x <0){
            angle = atan(posicio_apunta_z/posicio_apunta_x) +180.0*M_PI/180;
        }else{
            angle = 270.0f*M_PI/180;
        }
    }else if(posicio_apunta_z>0){
        if(posicio_apunta_x>0){
            angle = atan(posicio_apunta_z/posicio_apunta_x);
        }else if (posicio_apunta_x<0){
            angle = atan(posicio_apunta_z/posicio_apunta_x) +180*M_PI/180;
        }else{
            angle = 90.0f*M_PI/180;
        }
    }else{
        if(posicio_apunta_x <0){
            angle = 180.0f*M_PI/180;
        }else{
            angle = 0.0f*M_PI/180;
        }
    }


    switch(cas){
        case 3:
        angle -= 90.0f*M_PI/180;
        break;
        case 4:
        angle -= 90.0f*M_PI/180;
        break;
    }

    float posicio_x = cos(angle);
    float posicio_z = sin(angle);

    if(fabs(posicio_x)< limit){
        posicio_x = 0.0f;
    }

    if(fabs(posicio_z)< limit){
        posicio_z = 0.0f;
    }

    switch(cas){
        case 1:
        posicio_camera_x += 0.05*posicio_x;
        posicio_camera_z += 0.05*posicio_z;
        break;
        case 2:
        posicio_camera_x -= 0.05*posicio_x;
        posicio_camera_z -= 0.1*posicio_z;
        break;
        case 3:
        posicio_camera_x -= 0.05*posicio_x;
        posicio_camera_z -= 0.1*posicio_z;
        break;
        case 4:
        posicio_camera_x += 0.05*posicio_x;
        posicio_camera_z += 0.05*posicio_z;
        break;
    }

    tornar_lloc();

}

void volta_eixy(){


	double posi_x = posicio_camera_x;
	double posi_z = posicio_camera_z;
	int passes = 0;
	angleincrement = sentit*1*M_PI/180;

					posicio_camera_x = posi_x* cos(angleincrement) -posi_z* sin(angleincrement);
					posicio_camera_z = posi_x* sin(angleincrement) + posi_z* cos(angleincrement);
					glutPostRedisplay();
					sleep(0.1);

}

void volta_eixx(){


	double posi_z = posicio_camera_z;
	double posi_y = posicio_camera_y;
	int passes = 0;
	angleincrement = sentit*1*M_PI/180;

					posicio_camera_y = posi_y* cos(angleincrement) +posi_z* sin(angleincrement);
					posicio_camera_z = -posi_y* sin(angleincrement) + posi_z* cos(angleincrement);
					glutPostRedisplay();
					sleep(0.1);

}

void canviar_posicio_camera(){
	switch(posicio){
		case 0:
			posicio_camera_x = 0;
			posicio_camera_y = 3;
			posicio_camera_z = 0;

			posicio_apunta_x =0;
			posicio_apunta_y =0;
			posicio_apunta_z =0;

			verticaly =0;
			verticalz = -1;
			break;
		case 1:
			posicio_camera_x = 0;
			posicio_camera_y = 3;
			posicio_camera_z = 3;

			posicio_apunta_x =0;
			posicio_apunta_y =0;
			posicio_apunta_z =0;

			verticaly =1;
			verticalz = 0;
			break;
		case 2:
			posicio_camera_x = 0;
			posicio_camera_y = 0;
			posicio_camera_z = 5;

			posicio_apunta_x =0;
			posicio_apunta_y =0;
			posicio_apunta_z =0;

			verticaly =1;
			verticalz = 0;
			break;
		case 3:
			posicio_camera_x = 0;
			posicio_camera_y = -3;
			posicio_camera_z = 3;

			posicio_apunta_x =0;
			posicio_apunta_y =0;
			posicio_apunta_z =0;

			verticaly =1;
			verticalz = 0;
			break;
		case 4:
			posicio_camera_x = 0;
			posicio_camera_y = -3;
			posicio_camera_z = 0;

			posicio_apunta_x =0;
			posicio_apunta_y =0;
			posicio_apunta_z =0;

			verticaly =0;
			verticalz = 1;

			break;
	}
}

//Método para processar las teclas
void ProcessNormalKeys(unsigned char tecla, int x, int y){
    
		switch(tecla){
			case 'w':
				if(posicio ==3){
					moure_camera(1);
				}
				break;
			case 's':
				moure_camera(2);
					break;
			case 'd':
				if(posicio ==3){
					moure_camera(3);
				}
					break;
			case 'a':
				if(posicio ==3){
					moure_camera(4);
				}
					break;
			case 'r':
					sentit *= -1;
					break;
			case 'q':
					canviar_posicio_camera();
					posicio++;
					if(posicio >4){
						posicio =0;
					}
					break;
			case 'e':
					if(posicio ==3){
						{std::thread t1(volta_eixy);
						t1.join();}
					}
					break;
			case 'f':
					if(posicio ==3){
						{std::thread t2(volta_eixx);
						t2.join();}
					}
					break;
		}	
	
	
	glutPostRedisplay();
}



void Display(void)
{

  //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(posicio_camera_x, posicio_camera_y, posicio_camera_z, posicio_apunta_x, posicio_apunta_y, posicio_apunta_z, verticalx,verticaly,verticalz);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // 1.25 min per a z
  // 1.5 min per y
  glPushMatrix();
		glScalef(1.5,1.5,1.5);
		glBegin(GL_POLYGON);
			glColor4f(0.0f, 1.0f, 0.0f,1.0);
			glVertex3f(-0.25,-(R1+R2+0.25),-0.25);
            glColor4f(0.0f, 1.0f, 0.0f,1.0);
			glVertex3f(-0.25,-(R1+R2+0.25),0.25);
			glColor4f(0.0f, 1.0f, 0.0f,1.0);
			glVertex3f(0.25,-(R1+R2+0.25),0.25);
			glColor4f(0.0f, 1.0f, 0.0f,1.0);
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

	glLineWidth (1.0);
	glColor3f(1.0f, 0.0f, 1.0f);
    glutSolidSphere(0.03f,30.0,30.0);

	glutSwapBuffers();
	glFlush();
}

//Método que calcula el angulo de los brazos del péndulo
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


void init(){

	glutReshapeFunc(eventoVentana);
	glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_EQUAL);
	glutDisplayFunc(Display);
	glutIdleFunc(lagrange);
	glutSpecialFunc(ProcessSpecialKeys);
	glutKeyboardFunc(ProcessNormalKeys);

    printf(" flecha superior - enfocar la càmera cap a dalt\n");
    printf(" flecha inferior - enfocar la càmera cap a baix\n");
    printf("flecha esquerre - enfocar la càmera cap a l'esquerre\n");
    printf("flecha dreta - enfocar la càmera cap a la dreta\n");
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutCreateWindow("Pendulo");

	init();

	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f); // minims i màxims x,y i z que veim

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}