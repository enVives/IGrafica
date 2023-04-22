#include <GL/glut.h>
#include <stdio.h>
#include <iostream>

#define anchoVentana 512    /* ancho de la ventana */
#define altoVentana 512     /* alto de la ventana */
#define posicionVentanaX 50 /* pos. X de la esquina sup-izq de la ventana */
#define posicionVentanaY 50 /* pos. Y de la esquina sup-izq de la ventana */
#define alambre 1
#define solido 2

int modo = solido;
GLfloat angleinici = 20.0f;
GLfloat angleincrement = 0.0f;

/* Funci�n que establece la proyecci�n -------------------------------------*/
void proyeccion(void)
{

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // compara aquí amb ortho i frustrum
    glFrustum(-1, 1, -1, 1, 1.0, 5.0);
    //glOrtho (-1.2, 1.2, -1.2, 1.2, -1.2, 5.0);
}

/* Rutina asociada a eventos de ventana ------------------------------------*/
void eventoVentana(GLsizei ancho, GLsizei alto)
{

    glViewport(0, 0, ancho, alto);
    proyeccion();
}

/* Rutina de dibujo de la escena -------------------------------------------*/
void Dibuja()
{

    //glClearColor(.95f, .95f, .95f, 0.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, -2.25, 0, 0, 0, 0, 1, 0);
    glScalef(0.2, 0.2, 0.2);

    glPushMatrix();
    glRotatef(angleinici + angleincrement, 0.0, 1.0, 0.0);
    switch (modo)
    {
    case alambre:
        glColor3f(1.0, 0.0, 0.0);
        glutWireTorus(1.0, 5.0, 18.0, 50.0); // radi interior, radi exterior, ncares, nanells
        break;
    case solido:
        glColor3f(1.0, 0.0, 0.0);
        glutSolidTorus(1.0, 5.0, 18.0, 50.0); // radi interior, radi exterior, ncares, nanells
        break;
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(angleinici - angleincrement, 0.0, 1.0, 0.0);
    switch (modo)
    {
    case alambre:
        glColor3f(0.0, 1.0, 0.0);
        glutWireTorus(0.5, 2.5, 18.0, 20.0);
        break;
    case solido:
        glColor3f(0.0, 1.0, 0.0);
        glutSolidTorus(0.5, 2.5, 18.0, 20.0);
        break;
    }
    glPopMatrix();

    glPushMatrix();

    glRotatef(angleinici + angleincrement, 1.0, 0.0, 0.0);

    switch (modo)
    {
    case alambre:
        glColor3f(0.0, 0.0, 1.0);
        glutWireSphere(1.0, 30.0, 30.0);
        glColor3f(1.0, 1.0, 0.0);
        glutWireCone(1.0,2.0,10.0,2.0);
        break;
    case solido:
        glColor3f(0.0, 0.0, 1.0);
        glutSolidSphere(1.0, 30.0, 30.0);
        glColor3f(1.0, 1.0, 0.0);
        glutSolidCone(1.0,2.0,10.0,2.0);
        break;
    }

    glPopMatrix();

    glutSwapBuffers();
    glFlush();
}

/* Rutina asociada a eventos de teclado ------------------------------------*/
void eventoTeclado(unsigned char tecla, int x, int y)
{

    switch (tecla)
    {
    case 27:
        exit(0);
        break;
    case 'x':
        modo = solido;
        break;
    case 'z':
        modo = alambre;
        break;
    }
    glutPostRedisplay();
}
void Idle(void)
{
    // Incrementamos el �ngulo
    angleincrement += 0.3f;
    // Si es mayor que dos pi la decrementamos
    if ((angleincrement + angleinici) >= 360)
        angleincrement = 0;
    // Indicamos que es necesario repintar la pantalla
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
            glutIdleFunc(Idle);
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
        break;
    default:
        break;
    }
}

/* Opciones sobre la visualizaci�n de la escena ----------------------------*/
void opcionesVisualizacion(void)
{

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    printf(" z - Al�mbrico\n");
    printf(" x - S�lido \n");
    printf("click esquerre - moviment\n");
    printf("click dret - atura moviment \n");
}

/* Establece las funciones que atienden los distintos eventos --------------*/
void asociaEventos(void)
{

    glutReshapeFunc(eventoVentana);
    glutKeyboardFunc(eventoTeclado);
    glutMouseFunc(mouse);
}

/* Abre la ventana de la aplicaci�n ----------------------------------------*/
void abreVentana(int numParametros, char **listaParametros)
{

    glutInit(&numParametros, listaParametros);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(anchoVentana, altoVentana);
    glutInitWindowPosition(posicionVentanaX, posicionVentanaY);
    glutCreateWindow(listaParametros[0]);
}

/* Programa principal ------------------------------------------------------*/
int main(int numParametros, char **listaParametros)
{

    /* crea la ventana de la aplicaci�n*/
    abreVentana(numParametros, listaParametros);

    /* asocia funciones a eventos de ventana, teclado o rat�n*/
    asociaEventos();

    /* establece la funci�n de dibujo de la escena */
    glutDisplayFunc(Dibuja);

    /* establece opciones sobre la visualizaci�n de la escena */
    opcionesVisualizacion();
    /* bucle a la espera de eventos */
    glutMainLoop();
}