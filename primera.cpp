// Etapa1.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
const int W_WIDTH = 500; // Tama�o incial de la ventana
const int W_HEIGHT = 500;
GLfloat fAngulo; // Variable que indica el �ngulo de rotaci�n de los ejes.
GLfloat distancia;

// Funci�n que visualiza la escena OpenGL
void Display (void)
{
	// Borramos la escena
	//glClearColor(RGBA FLOAT) a quin color es netejarà la finestra
	glClear (GL_COLOR_BUFFER_BIT);

	glPushMatrix();
		// Rotamos las proximas primitivas
		
		glTranslatef(distancia,0.0,0.0);
		glRotatef (fAngulo, 0.0f, 0.0f, 1.0f);

		// Creamos a continuaci�n dibujamos los tres poligonos
		glBegin (GL_POLYGON);
			glColor3f (1.0f, 1.0f, 1.0f); // red, green, blue
			glVertex3f(0.0f, 0.0f, 0.0f);
			glColor3f (0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
			glColor3f (0.0f, 1.0f, 0.0f);
			glVertex3f(-0.5f, 0.866f, 0.0f);
		glEnd();

		
		glBegin (GL_POLYGON);
			glColor3f (1.0f, 1.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glColor3f (1.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
			glColor3f (0.0f, 0.0f, 1.0f);
			glVertex3f(-0.5f, -0.866f, 0.0f);
		glEnd();

		glBegin (GL_POLYGON);
			glColor3f (1.0f, 1.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glColor3f (0.0f, 1.0f, 1.0f);
			glVertex3f(-0.5f, 0.866f, 0.0f);
			glColor3f (0.0f, 0.0f, 1.0f);
			glVertex3f(-0.5f, -0.866f, 0.0f);
		glEnd();
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}


// Funci�n que se ejecuta cuando el sistema no esta ocupado
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



void mouse(int button, int state, int x, int y){
    switch(button){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN)
                glutIdleFunc(Idle);
            break;
        case GLUT_RIGHT_BUTTON:
            if(state == GLUT_DOWN)
                //glutIdleFunc(NULL);
            break;
		case GLUT_MIDDLE_BUTTON:
				if(state == GLUT_DOWN){
					distancia = distancia+0.25;
					glutPostRedisplay();
				}
			break;
        default:
            break;
    }

}


void Reproyectar(int width, int height){
	std::cout << "amplària "<<width << "\n";
	std::cout <<"altura "<<height << "\n";

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
// Funci�n principal
int main(int argc, char **argv) 
{
	// Inicializamos la librer�a GLUT
	glutInit (&argc,argv);

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition (100,100);
	glutInitWindowSize (W_WIDTH, W_HEIGHT);
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);

	// Creamos la nueva ventana
	glutCreateWindow ("Mi primera Ventana");
	
	// Indint width, int heighticamos cuales son las funciones de redibujado e idle
	glutDisplayFunc(Display);
	glutReshapeFunc(Reproyectar);
	glutMouseFunc(mouse);
	//glutIdleFunc(Idle);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor (1.0f, 1.0f, 1.0f, 1.0f); 
	glOrtho (-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f); //minims i màxims x,y i z que veim

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0; 
}
