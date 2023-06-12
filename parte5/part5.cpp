//Part 5
//Autors: Pere Joan Vives Morey i Arnau Vidal Moreno
//Codi per Executar : g++ part5.cpp -o part5 -lGL -lGLU -lglut
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

float posicio_camera_x = -7;
float posicio_camera_y =0;
float posicio_camera_z =0;
float angleincrement = 0.0f;

float posicio_bombilla_x = -3.0f;
float posicio_bombilla_y = 0.0f;
float posicio_bombilla_z = 0.0f;

float anglexz = 0.0;
float anglexy = 0.0;
int direccio =1;
int sentit = 1;

bool flat = false;
bool segona = false;

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
    GLfloat light_position [] = {posicio_bombilla_x,posicio_bombilla_y,posicio_bombilla_z,1.0f};
    GLfloat light_direction [] = {2.0f,0.0f,0};
    GLfloat white_light[] = {1.0f,1.0f,1.0f};
    GLfloat angle[] = {180.0};
    GLfloat lmodel_ambient[] = {0.1,0.1,0.1,1.0};

    if(flat){
        glShadeModel(GL_FLAT);
    }else{
        glShadeModel(GL_SMOOTH);
    }

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,white_light );
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);


    GLfloat light_ambient1[]   = { 0.05f, 0.0f, 0.0f, 1.0f };
    GLfloat light_diffuse1[]   = { 0.5f, 0.6f, 0.7f, 1.0f };
    GLfloat light_specular1[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position_dalt[]  = { -posicio_bombilla_x,-posicio_bombilla_y,-posicio_bombilla_z,1.0f};
    GLfloat light_direction1[] = { posicio_bombilla_x,posicio_bombilla_y,posicio_bombilla_z};
    GLfloat light_cutoff = 45.0f;


    glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position_dalt);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction1);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light_cutoff);

    //glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    if(segona){
        glEnable(GL_LIGHT1);
    }else{
        glDisable(GL_LIGHT1);
    }
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE); //per eliminar la cara de darrere
    //glEnable(GL_LIGHT1);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glEnable (GL_LINE_SMOOTH);
	glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST);

    GLfloat difusa[]=    {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat especular[]= {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat brillo[]=    {50.0f};

    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, difusa);
    glMaterialfv (GL_FRONT, GL_SPECULAR, especular);
    glMaterialfv (GL_FRONT, GL_SHININESS, brillo);

    glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable (GL_COLOR_MATERIAL);
}

void display(void){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    init();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    gluLookAt(posicio_camera_x, posicio_camera_y, posicio_camera_z, posicio_apunta_x, posicio_apunta_y, posicio_apunta_z, 0.0, 1.0, 0.0);
    
    int i;

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(-posicio_bombilla_x,-posicio_bombilla_y,-posicio_bombilla_z);
    glBegin(GL_POLYGON);
        if(segona){
           glColor3f(1.0,0,1); 
        }else{
            glColor3f(0.0,0.0,0);
        }
        glutSolidSphere(0.05,20,20);
    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
    
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

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(posicio_bombilla_x,posicio_bombilla_y,posicio_bombilla_z);
    glBegin(GL_POLYGON);
        glColor3f(1.0,0.0,0);
        glutSolidSphere(0.05,20,20);
    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
	glFlush();
}

void Reproyectar(int ancho, int alto){

    
    glViewport(0, 0, ancho, alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // compara aquí amb ortho i frustrum
	gluPerspective(45.0f,(GLdouble) ancho/alto , 0.0, 20.0);
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
    if(anglexz <0){
        gira_vista_y(-anglexz);
    }else{
        gira_vista_y(-anglexz);
    }
}

void ProcessSpecialKeys(int key, int x, int y)
{
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


void ProcessNormalKeys(unsigned char tecla, int x, int y){

	switch(tecla){
		case 'r':
				sentit *= -1;
				break;
		case 'e':
				volta_eixy();
				break;
        case 'p':
                flat = !flat;
                break;
        case 'a':
                posicio_bombilla_x += 0.01;
                break;
        case 'z':
                posicio_bombilla_x -= 0.01;
                break;
        case 's':
                posicio_bombilla_y += 0.01;
                break;
        case 'x':
                posicio_bombilla_y -= 0.01;
                break;
        case 'd':
                posicio_bombilla_z += 0.01;
                break;
        case 'c':
                posicio_bombilla_z -= 0.01;
                break;
        case 'q':
                segona = !segona;
                break;

	}
	glutPostRedisplay();
}

void opcionesVisualizacion(){
    printf(" flecha superior - enfocar la càmera cap a dalt\n");
    printf(" flecha inferior - enfocar la càmera cap a baix\n");
    printf("flecha esquerre - enfocar la càmera cap a l'esquerre\n");
    printf("flecha dreta - enfocar la càmera cap a la dreta\n");
    printf("e - fer voltar la figura a l'eix y\n");
    printf("r - canviar la direcció del gir produït per la tecla 'e'\n");
    printf("p - canviar flat/smooth\n");
    printf("a - avançar bombilla a l'eix x\n");
    printf("z - fer retrocedir la bombilla a l'eix x\n");
    printf("s - avançar la bombilla a l'eix y\n");
    printf("s- fer retrocedir la bombilla a l'eix y\n");
    printf("d - avançar la bombilla a l'eix z\n");
    printf("c - fer retrocedir la bombilla a l'eix z\n");
    printf("q - encendre/apagar segona bombilla\n");
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
    glutSpecialFunc(ProcessSpecialKeys);

    opcionesVisualizacion();
    //glutMouseFunc(mouse);
	// Indint width, int heighticamos cuales son las funciones de redibujado e idle
	//glutDisplayFunc(display);
	//glutIdleFunc(Idle);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glutMainLoop();
	return 0; 
}