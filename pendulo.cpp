// Doble Pèndul
// Simulació del doble Pèndul utilitzant les equacions de Lagrange
// Pere Joan Vives Morey i Arnau Vidal Moreno

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


void drawCircle(float cx, float cy, float r, int num_segments)
{
    float theta = 3.1415926 * 2 / float(num_segments);
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float x = r;//we start at angle = 0 

    float y = 0;
    glLineWidth(2);
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++)
    {
        glVertex2f(x + cx, y + cy);//output vertex 

        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float tx = -y;
        float ty = x;

        //add the tangential vector 

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        //correct using the radial factor 

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}

void Display(void)
{

  glClear(GL_COLOR_BUFFER_BIT);
  glLineWidth (5.0);

  float x1 = R1*sin(angle1);
  float y1 = -R1*cos(angle1);
  float x2 = R2*sin(angle2) + x1;
  float y2 = -R2*cos(angle2) + y1;

	glPushMatrix();
			glEnable (GL_LINE_SMOOTH);
			glEnable( GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST);

		glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(0.0f, 0.0f);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f((GLfloat)x1,(GLfloat)y1);	
		glEnd();


		glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(x1, y1);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f((GLfloat)x2,(GLfloat)y2);
		glEnd();
		
		glLineWidth (1.0);
		glBegin(GL_POLYGON);
		glColor3f(1.0f, 0.0f, 1.0f);
			drawCircle(x1,y1,0.03f,100);
		glEnd();

		glBegin(GL_POLYGON);
		glColor3f(1.0f, 0.0f, 1.0f);
			drawCircle(x2,y2,0.03f,100);
		glEnd();

		glBegin(GL_POLYGON);
		glColor3f(0.0f, 0.0f, 0.0f);
			drawCircle(0,0,0.03f,100);
		glEnd();

		glDisable (GL_LINE_SMOOTH);
		glDisable( GL_BLEND);
		
	glPopMatrix();

	glLineWidth (1.0);

	glutSwapBuffers();
	glFlush();
}
void lagrange(void){

	float a = -(M1+M2) *g*R1 * sin(angle1) - M2*R1*R2*pow(angle2dot,2)*sin(angle1-angle2);
	float b = (M1+M2) *pow(R1,2);
	float c = M1*R1*R2*cos(angle1-angle2);
	float f = -M2*g*R2*sin(angle2)+M2*R1*R2*pow(angle1dot,2)*sin(angle1-angle2);
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

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f); // minims i màxims x,y i z que veim

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}