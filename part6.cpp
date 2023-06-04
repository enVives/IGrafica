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

#define MAX_TEXTURAS       2  /* numero maximo de texturas */
#define ALTO_TEXTURA_PARED     1280   /* alto de la imagen a texturar */
#define ANCHO_TEXTURA_PARED    769  /* ancho de la imagen a texturar  */

#define ALTO_TEXTURA_SUELO     540  /* alto de la imagen a texturar */
#define ANCHO_TEXTURA_SUELO    360  /* ancho de la imagen a texturar  */


const int W_WIDTH = 500; // Tama�o incial de la ventana
const int W_HEIGHT = 500;

const float limit = 0.000001;

int alto_textura = ALTO_TEXTURA_PARED;
int ancho_textura = ANCHO_TEXTURA_PARED;

float posicio_apunta_x = 0.0f;
float posicio_apunta_y =0.5f;
float posicio_apunta_z =-1.0f;

float posicio_camera_x = 0.0f;
float posicio_camera_y =0.5f;
float posicio_camera_z =-0.5f;

float anglexz = 0.0;
float anglexy = 0.0;
int direccio =1;
int sentit = 1;

GLubyte textura_pared[ALTO_TEXTURA_PARED][ANCHO_TEXTURA_PARED][3]; /* vector de texturas */
GLubyte textura_enterra[ALTO_TEXTURA_SUELO][ANCHO_TEXTURA_SUELO][3]; /* vector de texturas */
GLuint nombreTexturas[MAX_TEXTURAS];



void crearParedes(){

        glPushMatrix();
        //paret B
        glBegin(GL_POLYGON);
        glNormal3f(-1.0,0.0,0.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,0.0, 0.0);
        glVertex3f(1.0,0.0,0.0);
        glNormal3f(-1.0,0.0,0.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,0.0, 1.0);
        glVertex3f(1.0,1.0,0.0);
        glNormal3f(-1.0,0.0,0.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,1.0, 1.0);
        glVertex3f(1.0,1.0,-5.0);
        glNormal3f(-1.0,0.0,0.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,1.0, 0.0);
        glVertex3f(1.0,0.0,-5.0);
        glEnd();

        //paret C
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,1.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,0.0, 0.0);
        glVertex3f(1.0,0.0,-5.0);
        glNormal3f(0.0,0.0,1.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,0.0, 1.0);
        glVertex3f(1.0,1.0,-5.0);
        glNormal3f(0.0,0.0,1.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,1.0, 1.0);
        glVertex3f(-1.0,1.0,-5.0);
        glNormal3f(0.0,0.0,1.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,1.0, 0.0);
        glVertex3f(-1.0,0.0,-5.0);
        glEnd();

        //paret D
        glBegin(GL_POLYGON);
        glNormal3f(1.0,0.0,0.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,0.0, 0.0);
        glVertex3f(-1.0,0.0,-5.0);
        glNormal3f(1.0,0.0,0.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,0.0, 1.0);
        glVertex3f(-1.0,1.0,-5.0);
        glNormal3f(1.0,0.0,0.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,1.0, 1.0);
        glVertex3f(-1.0,1.0,0.0);
        glNormal3f(1.0,0.0,0.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,1.0, 0.0);
        glVertex3f(-1.0,0.0,0.0);
        glEnd();

        //paret A
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,0.0, 0.0);
        glVertex3f(-1.0,0.0,0.0);
        glNormal3f(0.0,0.0,-1.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,0.0, 1.0);
        glVertex3f(-1.0,1.0,0.0);
        glNormal3f(0.0,0.0,-1.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,1.0, 1.0);
        glVertex3f(1.0,1.0,0.0);
        glNormal3f(0.0,0.0,-1.0);
        glMultiTexCoord2fARB(GL_TEXTURE0,1.0, 0.0);
        glVertex3f(1.0,0.0,0.0);
        glEnd();
        glPopMatrix();
        
        glPushMatrix();
        
        glBegin(GL_POLYGON);

        glNormal3f(0.0,1.0,0.0);
        
        glColor3f(1.0,1.0,1.0);
        glMultiTexCoord2fARB(GL_TEXTURE1,0.0, 0.0);
        glVertex3f(-1.0, 0.0, 0.0);
        glMultiTexCoord2fARB(GL_TEXTURE1,0.0, 1.0);
        glVertex3f(-1.0, 0.0,-5.0);
        glMultiTexCoord2fARB(GL_TEXTURE1,1.0, 1.0);
        glVertex3f(1.0, 0.0, -5.0);
        glMultiTexCoord2fARB(GL_TEXTURE1,1.0, 0.0);
        glVertex3f(1.0, 0.0,0.0);
        glEnd();

        glPopMatrix();
}

void Display(void){

    glClearColor (0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(posicio_camera_x, posicio_camera_y, posicio_camera_z, posicio_apunta_x, posicio_apunta_y, posicio_apunta_z, 0.0, 1.0, 0.0);


    crearParedes();

    glPushMatrix();
        glTranslatef(posicio_apunta_x,posicio_apunta_y,posicio_apunta_z);
        glColor3f(1.0f,1.0f,1.0f);
        glutSolidSphere(0.005,30.0,30.0);
    glPopMatrix();


    glutSwapBuffers();
	glFlush();
}

void eventoVentana(GLsizei ancho, GLsizei alto)
{

    glViewport(0, 0, ancho, alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // compara aquí amb ortho i frustrum
	gluPerspective(45.0f,(GLdouble) ancho/alto , 0.0, 10.0);
}

void leeTextura (char *fichero, int torn) {
    int   i, j;
    char  r, g, b, c;
    FILE  *tga;

    /* Apertura del fichero TGA */
    if ((tga = fopen(fichero, "rb")) == NULL)
        printf ("Error abriendo el fichero: %s\n", fichero);
    else {
        /* Lee los 18 primeros caracteres de la cabecera */
        for (j=1; j<=18; j++)
            fscanf (tga, "%c", &c);

        if(torn == 0){ //pared
            alto_textura = ALTO_TEXTURA_PARED;
            ancho_textura = ANCHO_TEXTURA_PARED;
        }else if(torn == 1){
            alto_textura = ALTO_TEXTURA_SUELO;
            ancho_textura = ANCHO_TEXTURA_SUELO;
        }
        /* Lee la imagen */
        for (j=alto_textura-1; j>=0; j--) {
            for (i=ancho_textura-1; i>=0; i--) {
                fscanf(tga, "%c%c%c", &b, &g, &r);
                if(torn ==0){
                 textura_pared[j][i][0] = (GLubyte)r;
                 textura_pared[j][i][1] = (GLubyte)g;
                 textura_pared[j][i][2] = (GLubyte)b;   
                }else if(torn == 1){
                 textura_enterra[j][i][0] = (GLubyte)r;
                 textura_enterra[j][i][1] = (GLubyte)g;
                 textura_enterra[j][i][2] = (GLubyte)b; 
                }
                
            }
        }
        fclose(tga);   /* Cierre del fichero TGA */
    }
}

void material (void) {
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(MAX_TEXTURAS, nombreTexturas);

  /* Asignaci�n de la imagen "pixels" como textura 2D */
  char nom [] = "pared.tga";
  leeTextura(nom,0);
  char nom1 [] = "enterre.tga";
  leeTextura(nom1,1);
  
  /* Definici�n de los par�metros iniciales de texturacion */ 
  glBindTexture(GL_TEXTURE_2D, nombreTexturas[0]); 

  
  glTexImage2D(GL_TEXTURE_2D, 0, 3, 1280, 769, 
	       0, GL_RGB, GL_UNSIGNED_BYTE, textura_pared);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
  glBindTexture(GL_TEXTURE_2D, nombreTexturas[1]);
  
  glTexImage2D(GL_TEXTURE_2D, 0, 3, 540, 360, 
	       0, GL_RGB, GL_UNSIGNED_BYTE, textura_enterra);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
  glActiveTextureARB (GL_TEXTURE0);
  glEnable        (GL_TEXTURE_2D);
  glBindTexture   (GL_TEXTURE_2D, nombreTexturas[0]);
  glTexEnvi       (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  
  /* unidad de textura 2 */
  glActiveTextureARB (GL_TEXTURE1);
  glEnable        (GL_TEXTURE_2D);
  glBindTexture   (GL_TEXTURE_2D, nombreTexturas[1]);
  glTexEnvi       (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void llum(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable (GL_LINE_SMOOTH);
    glEnable( GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
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


void ProcessNormalKeys(unsigned char tecla, int x, int y){
    

	switch(tecla){
		case 'w':
            moure_camera(1);
			break;
		case 's':
            moure_camera(2);
				break;
		case 'd':
            moure_camera(3);
				break;
		case 'a':
            moure_camera(4);
				break;
	}
	glutPostRedisplay();
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


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow("Part6");


    glutReshapeFunc(eventoVentana);
	glutDisplayFunc(Display);
	//glutIdleFunc(lagrange);
	glutSpecialFunc(ProcessSpecialKeys);
	glutKeyboardFunc(ProcessNormalKeys);


	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f); // minims i màxims x,y i z que veim

    opcionesVisualizacion();
    material();
    //llum();

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}

