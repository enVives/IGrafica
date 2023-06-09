//Part 5
//Autors: Pere Joan Vives Morey i Arnau Vidal Moreno
//Codi per Executar : g++ part5.cpp -o part5 -lGL -lGLU
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

#define X .525731112119133603
#define Z .850650808352039932

GLfloat fAngulo;
float posicio_apunta_x = 0;
float posicio_apunta_y =0;
float posicio_apunta_z =0;

float posicio_camera_x = 0;
float posicio_camera_y =0;
float posicio_camera_z =3;
float angleincrement = 0.0f;
int sentit = 1;

static GLfloat vdata [12][3] = {
    {-X,0.0,Z},{X,0.0,Z},{-X,0.0,-Z},{X,0.0,-Z},
    {0.0,Z,X},{0.0,Z,-X},{0.0,-Z,X},{0.0,-Z,-X},
    {Z,X,0.0},{-Z,X,0.0},{Z,-X,0.0},{-Z,-X,0.0}
};

static GLuint tindices[20][3] = {
    {1,4,0},{4,9,0},{4,5,9},{8,5,4},{1,8,4},
    {1,10,8},{10,3,8},{8,3,5},{3,2,5},{3,7,2},
    {3,10,7},{10,6,7},{6,11,7},{6,0,11},{6,1,0},
    {10,1,6},{11,0,9},{2,11,9},{5,2,9},{11,2,7}
};

void init(void){
    GLfloat light_position [] = {-10.0f,0.0f,0,1.0f};
    GLfloat light_direction [] = {2.0f,0.0f,0};
    GLfloat white_light[] = {1.0f,1.0f,1.0f};
    GLfloat angle[] = {180.0};
    GLfloat lmodel_ambient[] = {0.1,0.1,0.1,1.0};
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,white_light );
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
    //glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE); //per eliminar la cara de darrere
    //glEnable(GL_LIGHT1);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    GLfloat difusa[]=    {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat especular[]= {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat brillo[]=    {50.0f};

    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, difusa);
    glMaterialfv (GL_FRONT, GL_SPECULAR, especular);
    glMaterialfv (GL_FRONT, GL_SHININESS, brillo);

    glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable (GL_COLOR_MATERIAL);
}

void Idle (void)
{
	// Incrementamos el �ngulo
	fAngulo += 0.3f;
	// Si es mayor que dos pi la decrementamos
	if (fAngulo > 360)
		fAngulo -= 360;
	// Indicamos que es necesario repintar la pantalla
	glutPostRedisplay();
}
void display(void){
    
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Solució, colocar la llum cada vegada que cridam a display!!!!!!!!
    init();
    //aliasing
    //glEnable (GL_LINE_SMOOTH);
    //glEnable( GL_BLEND);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    gluLookAt(posicio_camera_x, posicio_camera_y, posicio_camera_z, posicio_apunta_x, posicio_apunta_y, posicio_apunta_z, 0.0, 1.0, 0.0);
  
    
    int i;

    //glPolygonMode(GL_FRONT, GL_LINE); //linies no poligons
    //glPolygonMode(GL_BACK, GL_LINE);
    
    glPushMatrix();
    glTranslatef(-2.0f,1.0f,-0.25f);
    glBegin(GL_POLYGON);
        glColor3f(1.0,0.0,0);
        glutSolidSphere(0.25,20,20);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glBegin(GL_LINES);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(-3,0,0);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(3,0,0);
    glEnd();
    glBegin(GL_LINES);
        glColor3f(0.0f,0,1.0f);
        glVertex3f(0,-3,0);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(0,3,0);
    glEnd();
    glBegin(GL_LINES);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(0,0,-3);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(0,0,3);
    glEnd();
    glPopMatrix();


    //glLineStipple(1,0x00FF); //linies stripped
    //glEnable(GL_LINE_STIPPLE);
    
    glPushMatrix();
    glRotatef(fAngulo,0.0f,1.0f,0.0f);
    glRotatef(45.0f,0.0f,0.0f,1.0f);

    
    glBegin(GL_TRIANGLES);
        for(i =0; i<20;i++){
            glColor3f(1.0,1.0,1.0);
            glNormal3fv(&vdata[tindices[i][0]][0]);
            //glEdgeFlag(GL_TRUE);
            glVertex3fv(&vdata[tindices[i][0]][0]);

            glColor3f(1.0,1.0,1.0);
            glNormal3fv(&vdata[tindices[i][1]][0]);
            //glEdgeFlag(GL_TRUE);
            glVertex3fv(&vdata[tindices[i][1]][0]);

            glColor3f(1.0,1.0,1.0);
            glNormal3fv(&vdata[tindices[i][2]][0]);
            //glEdgeFlag(GL_TRUE);
            glVertex3fv(&vdata[tindices[i][2]][0]);
        }
    glEnd();
    glPopMatrix();
    
    //glPolygonMode(GL_FRONT, GL_FILL); //linies no poligons
    //glPolygonMode(GL_BACK, GL_FILL);

    glutSwapBuffers();
	glFlush();

}

void Reproyectar(int ancho, int alto){

    
    glViewport(0, 0, ancho, alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // compara aquí amb ortho i frustrum
	gluPerspective(45.0f,(GLdouble) ancho/alto , 1.0, 20.0);
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


int main(int argc, char** argv){
    glutInit (&argc,argv);

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition (100,100);
	glutInitWindowSize (500, 500);
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE| GLUT_DEPTH);

	// Creamos la nueva ventana
	glutCreateWindow ("Arrays");

    init();
    glutDisplayFunc(display);
	glutReshapeFunc(Reproyectar);
    glutKeyboardFunc(ProcessNormalKeys);
    //glutMouseFunc(mouse);
	// Indint width, int heighticamos cuales son las funciones de redibujado e idle
	//glutDisplayFunc(display);
	//glutIdleFunc(Idle);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glutMainLoop();
	return 0; 
}