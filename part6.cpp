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

int alto_textura = ALTO_TEXTURA_PARED;
int ancho_textura = ANCHO_TEXTURA_PARED;

float posicio_apunta_x = 0;
float posicio_apunta_y =0;
float posicio_apunta_z =0;

float posicio_camera_x = 0;
float posicio_camera_y =0;
float posicio_camera_z =3;

float angleincrement = 0.0f;
int sentit = 1;

GLubyte textura_pared[ALTO_TEXTURA_PARED][ANCHO_TEXTURA_PARED][3]; /* vector de texturas */
GLubyte textura_enterra[ALTO_TEXTURA_SUELO][ANCHO_TEXTURA_SUELO][3]; /* vector de texturas */
GLuint nombreTexturas[MAX_TEXTURAS];



void crearParedes(){
    
        glPushMatrix();
        
        glBegin(GL_POLYGON);

        //glNormal3f(0.0,0.0,1.0);

        glMultiTexCoord2fARB(GL_TEXTURE1,0.0, 0.0);
        glVertex2f(-1.0, 0.0);
        glMultiTexCoord2fARB(GL_TEXTURE1,0.0, 1.0);
        glVertex2f(-1.0, 1.0);
        glMultiTexCoord2fARB(GL_TEXTURE1,1.0, 1.0);
        glVertex2f(1.0, 1.0);
        glMultiTexCoord2fARB(GL_TEXTURE1,1.0, 0.0);
        glVertex2f(1.0, 0.0);
        glEnd();

        glPopMatrix();
}

void Display(void){

    glClearColor (0.7,0.7,0.7,0.7);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(posicio_camera_x, posicio_camera_y, posicio_camera_z, posicio_apunta_x, posicio_apunta_y, posicio_apunta_z, 0.0, 1.0, 0.0);

    crearParedes();

    glutSwapBuffers();
	glFlush();
}

void eventoVentana(GLsizei ancho, GLsizei alto)
{

    glViewport(0, 0, ancho, alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // compara aquí amb ortho i frustrum
	gluPerspective(45.0f,(GLdouble) ancho/alto , 1.0, 10.0);
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
    llum();

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}


