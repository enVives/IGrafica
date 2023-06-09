/* -------------------------------------------------------------------------*/
/* Texturas 2D sobre primitiva GLUT                                         */
/* J. Ribelles                                                              */
/* -------------------------------------------------------------------------*/


#include <GL/glut.h>
#include <stdio.h>
#include <malloc.h>

#define anchoVentana     512  /* ancho de la ventana */
#define altoVentana      512  /* alto de la ventana */
#define posicionVentanaX  50  /* pos. X de la esquina sup-izq de la ventana */
#define posicionVentanaY  50  /* pos. Y de la esquina sup-izq de la ventana */
#define anchoTextura     640  /* ancho de la textura */
#define altoTextura      640  /* alto de la textura */

double aspecto;   /* relaci�n ancho/alto de la ventana */
GLubyte pixeles[anchoTextura][altoTextura][3];  /* Pixeles de la textura */
GLdouble  alfa = 0.0, beta = 0.0; /* giro de la escena */

/* Funci�n que establece la proyecci�n -------------------------------------*/
void proyeccion (void) {

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, aspecto, 1.0f, 20.0f);
}

/* Rutina asociada a eventos de ventana ------------------------------------*/
void eventoVentana (GLsizei ancho, GLsizei alto) {
  
  glViewport (0, 0, ancho, alto);
  aspecto= (GLdouble) ancho/alto;
  proyeccion();
}

/* Rutina asociada a eventos de teclado ------------------------------------*/
void eventoTeclado (unsigned char tecla, int x, int y) {

  switch (tecla) {
  case  27: exit (0); break;
  case 'q': exit (0); break;
  case 'Q': exit (0); break;
  }
}

/* Rutina asociada a eventos de teclado especial ---------------------------*/
void eventoTecladoEspecial (int tecla, int x, int y) {

  switch (tecla) {                
  case GLUT_KEY_UP : /* Pulsacion cursor arriba del teclado ampliado */
    beta = beta + 1.0;
    if (beta > 360.0) beta = beta - 360.0;
    break;
    
  case GLUT_KEY_DOWN : /* Pulsacion cursor abajo del teclado ampliado */
    beta = beta - 1.0;
    if (beta < 0.0) beta = beta + 360.0;
    break;
    
  case GLUT_KEY_RIGHT : /* Pulsacion cursor derecha del teclado ampliado */
    alfa = alfa + 1.0;
    if (alfa > 360.0) alfa = alfa - 360.0;
    break;
    
  case GLUT_KEY_LEFT : /* Pulsacion cursor izquierda del teclado ampliado */
    alfa = alfa - 1.0;
    if (alfa < 0.0) alfa = alfa + 360.0;
    break;
  }
  glutPostRedisplay ();
}

/* Rutina de dibujo de la escena -------------------------------------------*/
void Dibuja (void) {

  glClear (GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  
  glTranslatef (0.0, 0.0, -5.0);
  glRotated ( beta, 1.0,0.0,0.0);
  glRotated (-alfa, 0.0,1.0,0.0);

  glutSolidSphere (1.0, 90.0, 90.0);

  glutSwapBuffers ();
}

/* Definici�n de la textura ------------------------------------------------*/

/* Lee una imagen de textura en formato ppm y la almacena en la variable    */
/* global "pixeles"                                                          */
void leeTextura (char *fichero) {
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

        /* Lee la imagen */
        for (j=altoTextura-1; j>=0; j--) {
            for (i=anchoTextura-1; i>=0; i--) {
                fscanf(tga, "%c%c%c", &b, &g, &r);
                pixeles[j][i][0] = (GLubyte)r;
                 pixeles[j][i][1] = (GLubyte)g;
                 pixeles[j][i][2] = (GLubyte)b; 
            }
        }
        fclose(tga);   /* Cierre del fichero TGA */
    }
}

/* Definici�n de los materiales --------------------------------------------*/
void material (void) {

  GLfloat tparams[]={0,1.4,0,0.5};
  GLfloat sparams[]={0.5,0.0,0.0,0.5};

  GLfloat color_esfera_ambiente[]   = {0.5, 0.5, 0.5, 1.0};
  GLfloat color_esfera_difuso[]     = {0.8, 0.8, 0.8, 1.0};
  GLfloat color_esfera_especular[]  = {0.5, 0.5, 0.5, 1.0};
  GLfloat brillo_esfera_especular[] = {5.0};

  glMaterialfv(GL_FRONT, GL_AMBIENT,   color_esfera_ambiente);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   color_esfera_difuso);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  color_esfera_especular);
  glMaterialfv(GL_FRONT, GL_SHININESS, brillo_esfera_especular);
  char nom [] = "columna.tga";
  leeTextura(nom);

  /* Definici�n de los par�metros iniciales de texturacion */  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, anchoTextura, altoTextura, 
	       0, GL_RGB, GL_UNSIGNED_BYTE, pixeles);
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
  /* Fijar el modo de repeticion de textura inicial a GL_CLAMP */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  /* Fijar el filtro de textura inicial a GL_NEAREST */
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGenfv(GL_S, GL_OBJECT_PLANE, sparams);
  glTexGenfv(GL_T, GL_OBJECT_PLANE, tparams);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
}

/* Define las fuentes de luz -----------------------------------------------*/
void iluminacion() {

  GLfloat luz_ambiente[] = {0.2, 0.2, 0.2, 1.0};

  GLfloat luz_blanca_ambiente[]  = {0.1, 0.1, 0.1, 1.0};
  GLfloat luz_blanca_difusa[]    = {0.8, 0.8, 0.8, 1.0};
  GLfloat luz_blanca_especular[] = {0.4, 0.4, 0.4, 1.0};

  GLfloat luz_roja_ambiente[]  = {0.2, 0.0, 0.0, 1.0};
  GLfloat luz_roja_difusa[]    = {0.8, 0.3, 0.3, 1.0};
  GLfloat luz_roja_especular[] = {0.4, 0.1, 0.1, 1.0};

  GLfloat luz_posicion_baja[] = {0.0, 1.6, 1.6, 1.0};
  GLfloat luz_posicion_alta[] = {0.0, 0.8, 0.8, 1.0};

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luz_ambiente);

  glLightfv(GL_LIGHT0, GL_AMBIENT,  luz_blanca_ambiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  luz_blanca_difusa);
  glLightfv(GL_LIGHT0, GL_SPECULAR, luz_blanca_especular);
  glLightfv(GL_LIGHT0, GL_POSITION, luz_posicion_baja);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,    0.0);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

  glEnable  (GL_LIGHTING);
  glEnable  (GL_LIGHT0);
}

/* Opciones sobre la visualizaci�n de la escena ----------------------------*/
void opcionesVisualizacion (void) {

  glCullFace    (GL_BACK);
  glEnable      (GL_CULL_FACE);
  glShadeModel  (GL_SMOOTH);
  glEnable      (GL_NORMALIZE);
  glClearColor  (1.0, 1.0, 1.0, 1.0);
}

/* Establece las funciones que atienden los distintos eventos --------------*/
void  asociaEventos (void) {

  glutReshapeFunc  (eventoVentana);
  glutKeyboardFunc (eventoTeclado);
  glutSpecialFunc  (eventoTecladoEspecial);
}

/* Abre la ventana de la aplicaci�n ----------------------------------------*/
void abreVentana (int numParametros, char **listaParametros) {

  glutInit               (&numParametros, listaParametros);
  glutInitDisplayMode    (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize     (anchoVentana, altoVentana);
  glutInitWindowPosition (posicionVentanaX, posicionVentanaY);
  glutCreateWindow       (listaParametros[0]);
}

/* Programa principal ------------------------------------------------------*/
int main (int numParametros, char **listaParametros) {

  /* crea la ventana de la aplicaci�n*/
  abreVentana (numParametros, listaParametros);

  /* asocia funciones a eventos de ventana, teclado o rat�n*/
  asociaEventos ();              

  /* establece la funcion de dibujo de la escena */
  glutDisplayFunc (Dibuja);

  /* establece opciones sobre la visualizaci�n de la escena */
  opcionesVisualizacion();

  /* define las fuentes de luz */
  iluminacion();

  /* define los materiales */
  material ();

  /* bucle a la espera de eventos */
  glutMainLoop();
}
