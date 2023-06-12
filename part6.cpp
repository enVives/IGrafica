#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#define MAX_TEXTURAS       6 /* numero maximo de texturas */
#define ALTO_TEXTURA_PARED     1280   /* alto de la imagen a texturar */
#define ANCHO_TEXTURA_PARED    769  /* ancho de la imagen a texturar  */

#define ALTO_TEXTURA_SUELO     540  /* alto de la imagen a texturar */
#define ANCHO_TEXTURA_SUELO    360  /* ancho de la imagen a texturar  */

#define ALTO_TEXTURA_SOSTRE     600  /* alto de la imagen a texturar */
#define ANCHO_TEXTURA_SOSTRE   600  /* ancho de la imagen a texturar  */

#define ALTO_TEXTURA_MARIE     539  /* alto de la imagen a texturar */
#define ANCHO_TEXTURA_MARIE  902  /* ancho de la imagen a texturar  */

#define ALTO_TEXTURA_METAL     178  /* alto de la imagen a texturar */
#define ANCHO_TEXTURA_METAL  283  /* ancho de la imagen a texturar  */


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

//Per els llums

float angle1_llum = 0.0f;
float angle2_llum = 0.0f;

float altura_llums= 0.5f;

bool ences_esquerre = true;
bool ences_dreta = true;
bool ences_quadre = false;
bool fusio = true;
bool acaba = false;
bool dedins = true;


GLubyte textura_pared[ALTO_TEXTURA_PARED][ANCHO_TEXTURA_PARED][3]; /* vector de texturas */
GLubyte textura_enterra[ALTO_TEXTURA_SUELO][ANCHO_TEXTURA_SUELO][3]; /* vector de texturas */
GLubyte textura_sostre[ALTO_TEXTURA_SOSTRE][ANCHO_TEXTURA_SOSTRE][3]; /* vector de texturas */
GLubyte textura_marie[ALTO_TEXTURA_MARIE][ANCHO_TEXTURA_MARIE][3]; /* vector de texturas */
GLubyte textura_metal[ALTO_TEXTURA_METAL][ANCHO_TEXTURA_METAL][3]; /* vector de texturas */
GLuint nombreTexturas[MAX_TEXTURAS];


static GLfloat posicionsbolles [7][3] = {{-0.18,0.33,-2.5},{0.0,0.33,-2.51},{0,0.33,-2.49},
    {0.01,0.33,-2.51},{0.01,0.33,-2.49},{0.005,0.33,-2.53},{0.005,0.33,-2.47},};
static GLfloat paretsdata [24][3] = {{-1,0,0},{-1,1,0},{1,1,0},{1,0,0},
    {1,0,-1},{1,1,-1},{1,1,-2},{1,0,-2},{1,0,-3},{1,1,-3},{1,1,-4},{1,0,-4},
    {1,0,-5},{1,1,-5},{-1,1,-5},{-1,0,-5},{-1,0,-4},{-1,1,-4},{-1,1,-3},{-1,0,-3},
    {-1,0,-2},{-1,1,-2},{-1,1,-1},{-1,0,-1}
    };

static GLuint paretsindexs[12][4] = {{0,1,2,3},{2,3,4,5},{4,5,6,7},{6,7,8,9},
    {8,9,10,11},{10,11,12,13},{12,13,14,15},{14,15,16,17},{16,17,18,19},
    {18,19,20,21},{20,21,22,23},{22,23,0,1}};

static GLfloat normaleParets[24][3] = {{1,0,-1},{1,0,-1},{-1,0,-1},{-1,0,-1},
    {-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},
    {-1,0,1},{-1,0,1},{1,0,1},{1,0,1},{1,0,0},{1,0,0},{1,0,0},{1,0,0},
    {1,0,0},{1,0,0},{1,0,0},{1,0,0}
    };

static GLfloat sostredata[33][3] = {{-1,1,0},{0,1,0},{0,1,-0.5},{-1,1,-0.5},
    {1,1,0},{1,1,-0.5},{-1,1,-1},{0,1,-1},{1,1,-1},{-1,1,-1.5},{0,1,-1.5},{1,1,-1.5},
    {-1,1,-2},{0,1,-2},{1,1,-2},{-1,1,-2.5},{0,1,-2.5},{1,1,-2.5},{-1,1,-3},{0,1,-3},
    {1,1,-3},{-1,1,-3.5},{0,1,-3.5},{1,1,-3.5},{-1,1,-4},{0,1,-4},{1,1,-4},{-1,1,-4.5},
    {0,1,-4.5},{1,1,-4.5},{-1,1,-5},{0,1,-5},{1,1,-5}

};

static GLuint sostreindexs[20][4] = {{0,1,2,3},{3,2,7,6},{6,7,10,9},{9,10,13,12},{12,13,16,15},{15,16,19,18},
    {18,19,22,21},{21,22,25,24},{24,25,28,27},{27,28,31,30},{1,4,5,2},{2,5,8,7},{7,8,11,10},{10,11,14,13},{13,14,17,16},
    {16,17,20,19},{19,20,23,22},{22,23,26,25},{25,26,29,28},{28,29,32,31}
};

static GLfloat enterredata[33][3] = {{-1,0,0},{0,0,0},{0,0,-0.5},{-1,0,-0.5},
    {1,0,0},{1,0,-0.5},{-1,0,-1},{0,0,-1},{1,0,-1},{-1,0,-1.5},{0,0,-1.5},{1,0,-1.5},
    {-1,0,-2},{0,0,-2},{1,0,-2},{-1,0,-2.5},{0,0,-2.5},{1,0,-2.5},{-1,0,-3},{0,0,-3},
    {1,0,-3},{-1,0,-3.5},{0,0,-3.5},{1,0,-3.5},{-1,0,-4},{0,0,-4},{1,0,-4},{-1,0,-4.5},
    {0,0,-4.5},{1,0,-4.5},{-1,0,-5},{0,0,-5},{1,0,-5}

};

static GLuint enterreindexs[20][4] = {{0,1,2,3},{3,2,7,6},{6,7,10,9},{9,10,13,12},{12,13,16,15},{15,16,19,18},
    {18,19,22,21},{21,22,25,24},{24,25,28,27},{27,28,31,30},{1,4,5,2},{2,5,8,7},{7,8,11,10},{10,11,14,13},{13,14,17,16},
    {16,17,20,19},{19,20,23,22},{22,23,26,25},{25,26,29,28},{28,29,32,31}
};

GLUquadricObj *objCylinder = gluNewQuadric();

void drawCylinder(float radius, float height, int numSegments) {
    float segmentHeight = height / numSegments;
    float segmentAngle = 2.0f * M_PI / numSegments;

    for (int i = 0; i < numSegments; i++) {
        float angle1 = i * segmentAngle;
        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        float y1 = -height / 2.0f;

        float angle2 = (i + 1) * segmentAngle;
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);
        float y2 = height / 2.0f;

        // Draw top face
        glBegin(GL_TRIANGLES);
        glColor3f(0.0,0.0,0.0);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0f, y2, 0.0f);
        glTexCoord2f(0.0,1.0);
        glVertex3f(x1, y2, z1);
        glTexCoord2f(1.0,0.0);
        glVertex3f(x2, y2, z2);
        glEnd();

        // Draw bottom face
        glBegin(GL_TRIANGLES);
        glColor3f(0.0,0.0,0.0);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0f, y1, 0.0f);
        glTexCoord2f(0.0,1.0);
        glVertex3f(x2, y1, z2);
        glTexCoord2f(1.0,0.0);
        glVertex3f(x1, y1, z1);
        glEnd();

        // Draw side face
        glBegin(GL_QUADS);
        glColor3f(0.0,0.0,0.0);
        glTexCoord2f(0.0,0.0);
        glVertex3f(x1, y1, z1);
        glTexCoord2f(0.0,1.0);
        glVertex3f(x2, y1, z2);
        glTexCoord2f(1.0,1.0);
        glVertex3f(x2, y2, z2);
        glTexCoord2f(1.0,0.0);
        glVertex3f(x1, y2, z1);
        glEnd();
    }
}
void crearParedes(){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, nombreTexturas[0]);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glPushMatrix();
        for(int i=0;i<12;i++){
            glBegin(GL_POLYGON);
                    glColor3f(0.7,0.7,0.7);
                    glTexCoord2f(0.0,0.0);
                    glNormal3fv(&normaleParets[paretsindexs[i][0]][0]);
                    glVertex3fv(&paretsdata[paretsindexs[i][0]][0]);

                    glColor3f(0.7,0.7,0.7);
                    glTexCoord2f(0.0,1.0);
                    glNormal3fv(&normaleParets[paretsindexs[i][0]][0]);
                    glVertex3fv(&paretsdata[paretsindexs[i][1]][0]);

                    glColor3f(0.7,0.7,0.7);
                    glTexCoord2f(1.0,1.0);
                    glNormal3fv(&normaleParets[paretsindexs[i][0]][0]);
                    glVertex3fv(&paretsdata[paretsindexs[i][2]][0]);

                    glColor3f(0.7,0.7,0.7);
                    glTexCoord2f(1.0,0.0);
                    glNormal3fv(&normaleParets[paretsindexs[i][0]][0]);
                    glVertex3fv(&paretsdata[paretsindexs[i][3]][0]);
            glEnd();
        }
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, nombreTexturas[1]);

        glPushMatrix();
        for(int i=0;i<20;i++){
        glBegin(GL_POLYGON);
                    glTexCoord2f(0.0,0.0);
                    glNormal3f(0,1,0);
                    glVertex3fv(&enterredata[enterreindexs[i][0]][0]);

                    glTexCoord2f(0.0,1.0);
                    glNormal3f(0,1,0);
                    glVertex3fv(&enterredata[enterreindexs[i][1]][0]);

                    glTexCoord2f(1.0,1.0);
                    glNormal3f(0,1,0);
                    glVertex3fv(&enterredata[enterreindexs[i][2]][0]);

                    glTexCoord2f(1.0,0.0);
                    glNormal3f(0,1,0);
                    glVertex3fv(&enterredata[enterreindexs[i][3]][0]);
        glEnd();
        }
    glPopMatrix();
        //corregir

        /*
        glBindTexture(GL_TEXTURE_2D, nombreTexturas[1]);

        glPushMatrix();
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

            glTranslated(-1,0,0);
            glRotatef(-90.0,1,0,0);
            gluCylinder(objCylinder,0.10,0.10,1,12,8);

            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
        glPopMatrix();*/
    glDisable(GL_TEXTURE_2D);

}

void pintarSostre(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nombreTexturas[2]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glPushMatrix();
        for(int i=0;i<20;i++){
        glBegin(GL_POLYGON);
            glColor3f(0.7,0.7,0.7);
                    glTexCoord2f(0.0,0.0);
                    glNormal3f(0,-1,0);
                    glVertex3fv(&sostredata[sostreindexs[i][0]][0]);

                    glTexCoord2f(0.0,1.0);
                    glNormal3f(0,-1,0);
                    glVertex3fv(&sostredata[sostreindexs[i][1]][0]);

                    glTexCoord2f(1.0,1.0);
                    glNormal3f(0,-1,0);
                    glVertex3fv(&sostredata[sostreindexs[i][2]][0]);

                    glTexCoord2f(1.0,0.0);
                    glNormal3f(0,-1,0);
                    glVertex3fv(&sostredata[sostreindexs[i][3]][0]);
        glEnd();
        }
    glPopMatrix();
}

void pintarColumna(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nombreTexturas[2]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glPushMatrix();
        glTranslatef(0,0,-2.5);
        glRotatef(-90.0,1,0,0);
        glutSolidCone(0.1,0.3,30,30);
    glPopMatrix();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
        glBegin(GL_POLYGON);
            glNormal3f(0,1,0);
            glColor3f(0.4,0.4,0.4);
            glVertex3f(-0.2,0.3,-2.3);
            glNormal3f(0,1,0);
            glColor3f(0.4,0.4,0.4);
            glVertex3f(0.2,0.3,-2.3);
            glNormal3f(0,1,0);
            glColor3f(0.4,0.4,0.4);
            glVertex3f(0.2,0.3,-2.7);
            glNormal3f(0,1,0);
            glColor3f(0.4,0.4,0.4);
            glVertex3f(-0.2,0.3,-2.7);
        glEnd();
    glPopMatrix();
}

void pintarQuadre(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nombreTexturas[1]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glPushMatrix();
        glTranslatef(0,0,-5);
        
        glBegin(GL_POLYGON);
            glTexCoord2f(0.0,0.0);
            glColor3f(0.4,0.2,0.1);
            glNormal3f(0,0,1);
            glVertex3f(-0.5,0.25,0);
            glNormal3f(0,0,1);
            glTexCoord2f(0.0,1.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(0.5,0.25,0);
            glNormal3f(0,0,1);
            glTexCoord2f(1.0,1.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(0.5,0.3,0);
            glNormal3f(0,0,1);
            glTexCoord2f(1.0,0.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(-0.5,0.3,0);
        glEnd();
        glBegin(GL_POLYGON);
            glNormal3f(0,0,1);
            glTexCoord2f(0.0,0.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(-0.5,0.3,0);
            glNormal3f(0,0,1);
            glTexCoord2f(0.0,1.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(-0.45,0.3,0);
            glNormal3f(0,0,1);
            glTexCoord2f(1.0,1.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(-0.45,0.7,0);
            glNormal3f(0,0,1);
            glTexCoord2f(1.0,0.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(-0.5,0.7,0);
        glEnd();
        glBegin(GL_POLYGON);
            glNormal3f(0,0,1);
            glTexCoord2f(0.0,0.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(-0.5,0.7,0);
            glNormal3f(0,0,1);
            glTexCoord2f(0.0,1.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(0.5,0.7,0);
            glNormal3f(0,0,1);
            glTexCoord2f(1.0,1.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(0.5,0.75,0);
            glNormal3f(0,0,1);
            glTexCoord2f(1.0,0.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(-0.5,0.75,0);
        glEnd();
        glBegin(GL_POLYGON);
            glNormal3f(0,0,1);
            glTexCoord2f(0.0,0.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(0.5,0.3,0);
            glNormal3f(0,0,1);
            glTexCoord2f(0.0,1.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(0.45,0.3,0);
            glNormal3f(0,0,1);
            glTexCoord2f(1.0,1.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(0.45,0.7,0);
            glNormal3f(0,0,1);
            glTexCoord2f(1.0,0.0);
            glColor3f(0.4,0.2,0.1);
            glVertex3f(0.5,0.7,0);
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, nombreTexturas[3]);

        glBegin(GL_POLYGON);
            glNormal3f(0,0,1);
            glTexCoord2f(0.0,0.0);
            glVertex3f(0.45,0.3,0);
            glNormal3f(0,0,1);
            glTexCoord2f(0.0,1.0);
            glVertex3f(0.45,0.7,0);
            glNormal3f(0,0,1);
            glTexCoord2f(1,1);
            glVertex3f(-0.45,0.7,0);
            glNormal3f(0,0,1);
            glTexCoord2f(1,0.0);
            glVertex3f(-0.45,0.3,0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void pintarLlum(){
    //Braç 1
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nombreTexturas[4]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glPushMatrix();
        glTranslatef(-0.1,0.875,-2.5);
        glTranslatef(0,0.125,0);
        glRotatef(-angle1_llum,0,0,1);
        glTranslatef(0,-0.125,0);

        glBegin(GL_POLYGON);
            drawCylinder(0.01,0.25,30);
        glEnd();
    glPopMatrix();
    //Braç 2
    glPushMatrix();
        glTranslatef(0.1,0.875,-2.5);
        glTranslatef(0,0.125,0);
        glRotatef(angle1_llum,0,0,1);
        glTranslatef(0,-0.125,0);

        glBegin(GL_POLYGON);
            drawCylinder(0.01,0.25,30);
        glEnd();
    glPopMatrix();
    //Braç 3
    glPushMatrix();
        glTranslatef(-0.1,0.625,-2.5);
        glTranslatef(0,0.375,0);
        glRotatef(-angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(angle2_llum+angle1_llum,0,0,1);
        glTranslatef(0,-0.125,0);
        glBegin(GL_POLYGON);
            drawCylinder(0.01,0.25,30);
        glEnd();
    glPopMatrix();
    //Braç 4
    glPushMatrix();
        glTranslatef(0.1,0.625,-2.5);

        glTranslatef(0,0.375,0);
        glRotatef(angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(-angle2_llum-angle1_llum,0,0,1);
        glTranslatef(0,-0.125,0);
        glBegin(GL_POLYGON);
            drawCylinder(0.01,0.25,30);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //Bolla 2 braç 1
    glPushMatrix();
        glTranslatef(-0.1,0.75,-2.5);
        glTranslatef(0.0,0.25,0);
        glRotatef(-angle1_llum,0,0,1);
        glTranslatef(0.0,-0.25,0);

        glBegin(GL_POLYGON);
            glColor3f(0.0,0.0,0.0);
            glutSolidSphere(0.01,30,30);
        glEnd();
    glPopMatrix();

    //Bolla 2 braç 2
    glPushMatrix();
        glTranslatef(0.1,0.75,-2.5);
        glTranslatef(0.0,0.25,0);
        glRotatef(angle1_llum,0,0,1);
        glTranslatef(0.0,-0.25,0);
        glBegin(GL_POLYGON);
            glColor3f(0.0,0.0,0.0);
            glutSolidSphere(0.01,30,30);
        glEnd();
    glPopMatrix();

    //Bolla 1 braç 1
    glPushMatrix();
        glTranslatef(-0.1,1,-2.5);
        glBegin(GL_POLYGON);
            glColor3f(0.0,0.0,0.0);
            glutSolidSphere(0.01,30,30);
        glEnd();
    glPopMatrix();

    //Bolla 1 braç 2
    glPushMatrix();
        glTranslatef(0.1,1,-2.5);
        glBegin(GL_POLYGON);
            glColor3f(0.0,0.0,0.0);
            glutSolidSphere(0.01,30,30);
        glEnd();
    glPopMatrix();

    //bombilla esquerre
    glPushMatrix();
    glTranslatef(-0.1,0.48,-2.5);
        glTranslatef(0,0.5,0);
        glRotatef(-angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(angle2_llum+angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(-90.0,1,0,0);
        glBegin(GL_POLYGON);
            if(ences_esquerre){
                glColor3f(1.0,1.0,1.0);
            }else{
                glColor3f(0.0,0.0,0.0);
            }
            glutSolidSphere(0.015,30,30);
        glEnd();
    glPopMatrix();

    //bombilla dreta
    glPushMatrix();
    glTranslatef(0.1,0.48,-2.5);
        glTranslatef(0,0.5,0);
        glRotatef(angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(-angle2_llum-angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(-90.0,1,0,0);
        glBegin(GL_POLYGON);
            if(ences_dreta){
                glColor3f(1.0,1.0,1.0);
            }else{
                glColor3f(0.0,0.0,0.0);
            }
            glutSolidSphere(0.015,30,30);
        glEnd();
    glPopMatrix();

    //cono de llum esquerre
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nombreTexturas[4]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glPushMatrix();
        glTranslatef(-0.1,0.48,-2.5);

        
        glTranslatef(0,0.5,0);
        glRotatef(angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(-angle2_llum-angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(angle2_llum,0,0,1);

        glRotatef(-90.0,1,0,0);
        glBegin(GL_POLYGON);
            glColor3f(0.0,0.0,0.0);
            glutSolidCone(0.025,0.03,30,30);
        glEnd();
    glPopMatrix();

    //cono de llum dret
    glPushMatrix();
        glTranslatef(0.1,0.48,-2.5);

        glTranslatef(0,0.5,0);
        glRotatef(-angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(angle2_llum+angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(-angle2_llum,0,0,1);
        glRotatef(-90.0,1,0,0);

        glBegin(GL_POLYGON);
            glColor3f(0.0,0.0,0.0);
            glutSolidCone(0.025,0.03,30,30);
        glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);

    //Bolla braç 3
    glPushMatrix();
        glTranslatef(-0.1,0.5,-2.5);
        glTranslatef(0,0.5,0);
        glRotatef(-angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(angle2_llum+angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glBegin(GL_POLYGON);
            glColor3f(0.0,0.0,0.0);
            glutSolidSphere(0.01,30,30);
        glEnd();
    glPopMatrix();

    //Bolla braç 4
    glPushMatrix();
        glTranslatef(0.1,0.5,-2.5);
        glTranslatef(0,0.5,0);
        glRotatef(angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glRotatef(-angle2_llum-angle1_llum,0,0,1);
        glTranslatef(0,-0.25,0);
        glBegin(GL_POLYGON);
            glColor3f(0.0,0.0,0.0);
            glutSolidSphere(0.01,30,30);
        glEnd();
    glPopMatrix();
}

void pintarLLumQuadre(){
    glPushMatrix();
        glBegin(GL_POLYGON);
            if(ences_quadre){
                glColor3f(1,1,1);
            }else{
                glColor3f(0,0,0);
            }
            glVertex3f(-0.25,0.01,-4.75);
            glVertex3f(-0.25,0.01,-5);
            glVertex3f(0.25,0.01,-5);
            glVertex3f(0.25,0.01,-4.75);
        glEnd();
    glPopMatrix();
}

void menuapp(int value) {

  switch(value) {
  case 1: fusio = true;
	break;
  case 2:  fusio = false;  
  static GLfloat posicionsbolles1 [7][3] = {{-0.18,0.33,-2.5},{0.0,0.33,-2.51},{0,0.33,-2.49},
    {0.01,0.33,-2.51},{0.01,0.33,-2.49},{0.005,0.33,-2.53},{0.005,0.33,-2.47},};
    memcpy(posicionsbolles, posicionsbolles1, sizeof(posicionsbolles));
  break;
  case 3:  exit(2); break;
  case 4:  break;
  default:                             break;
  }
  glutPostRedisplay();

}

void init(void){
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glDepthFunc   (GL_LEQUAL);
    glEnable(GL_NORMALIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_STENCIL_TEST);

    glEnable (GL_LINE_SMOOTH);
	glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST);
    //fins aquí va bé

    glEnable(GL_LIGHTING);{
        GLfloat light_ambient[]   = { 0.05f, 0.0f, 0.0f, 1.0f };
        GLfloat light_diffuse[]   = { 0.5f, 0.6f, 0.7f, 1.0f };
        GLfloat light_specular[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat light_position_esquerre[]  = { -0.1f, altura_llums, -2.5f, 1.0f };
        GLfloat light_position_dreta[]  = { 0.1f, altura_llums, -2.5f, 1.0f };
        GLfloat light_position_quadre[]  = { 0.0f, 0.01, -4.75f, 1.0f };
        GLfloat light_direction[] = { 0.0f,-1.0f,0};
        GLfloat light_direction_quadre[] = { 0.0f,1.0f,-0.5};
        GLfloat light_cutoff = 60.0f;
        GLfloat light_cutoff_quadre =90.0f;
        
        glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position_esquerre);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, light_cutoff);
        glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0);
        glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.5);
        glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.2);



        glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT1, GL_POSITION, light_position_dreta);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light_cutoff);
        glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,1.0);
        glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,0.5);
        glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,0.2);

        glLightfv(GL_LIGHT2, GL_AMBIENT,  light_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE,  light_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT2, GL_POSITION, light_position_quadre);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction_quadre);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, light_cutoff_quadre);
        glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION,1.0);
        glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION,0.5);
        glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,0.2);

        //GLfloat mat_ambient[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
        //GLfloat mat_diffuse[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
        //GLfloat mat_specular[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat mat_shininess[] = { 50.0 };

        GLfloat mat_specular[] = {1.0, 0.901, 0.792, 1.0};
        GLfloat mat_diffuse[] = {1.0, 0.901, 0.792, 1.0};
        GLfloat mat_ambient[] = {1.0, 0.901, 0.792, 1.0};
        GLfloat mat_emission[] = {0.3,0.3,0.2,0.0};

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);


        if(ences_esquerre){
            glEnable(GL_LIGHT0);
        }else{
            glDisable(GL_LIGHT0);
        }

        if(ences_dreta){
            glEnable(GL_LIGHT1);
        }else{
            glDisable(GL_LIGHT1);
        }

        if(ences_quadre){
            glEnable(GL_LIGHT2);
        }else{
            glDisable(GL_LIGHT2);
        }

        glEnable(GL_FOG);
        {
        GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, 0.04);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        }

        /*
        int submenu = glutCreateMenu(menuapp);
        glutAddMenuEntry("Incrementar", 3);
        glutAddMenuEntry("Decrementar", 4);*/
    }
}

void fisio(void){

    glPushMatrix();
        GLfloat mat_emission5[] = {0.0,1.0,0.0,0.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission5);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
        glTranslatef(posicionsbolles[5][0],posicionsbolles[5][1],posicionsbolles[5][2]);
        glBegin(GL_POLYGON);
            glColor3f(0,1,0);
            glutSolidSphere(0.005,30,30);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        GLfloat mat_emission[] = {1.0,0.0,0.0,0.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
        glTranslatef(posicionsbolles[0][0],posicionsbolles[0][1],posicionsbolles[0][2]);
        glBegin(GL_POLYGON);
            glColor3f(1,0,0);
            glutSolidSphere(0.005,30,30);
        glEnd();
    glPopMatrix();
    
    glPushMatrix();
        GLfloat mat_emission1[] = {1.0,0.0,1.0,0.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission1);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
        glTranslatef(posicionsbolles[1][0],posicionsbolles[1][1],posicionsbolles[1][2]);
        glBegin(GL_POLYGON);
            glColor3f(1,0,1);
            glutSolidSphere(0.005,30,30);
        glEnd();
    glPopMatrix();
    glPushMatrix();
        GLfloat mat_emission2[] = {1.0,0.0,1.0,0.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission2);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
        glTranslatef(posicionsbolles[2][0],posicionsbolles[2][1],posicionsbolles[2][2]);
        glBegin(GL_POLYGON);
            glColor3f(1,0,1);
            glutSolidSphere(0.005,30,30);
        glEnd();
    glPopMatrix();
    glPushMatrix();
        GLfloat mat_emission3[] = {0.0,0.0,0.0,0.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission3);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
        glTranslatef(posicionsbolles[3][0],posicionsbolles[3][1],posicionsbolles[3][2]);
        glBegin(GL_POLYGON);
            glColor3f(0,0,0);
            glutSolidSphere(0.005,30,30);
        glEnd();
    glPopMatrix();
    glPushMatrix();
        GLfloat mat_emission4[] = {0.0,0.0,0.0,0.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission4);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
        glTranslatef(posicionsbolles[4][0],posicionsbolles[4][1],posicionsbolles[4][2]);
        glBegin(GL_POLYGON);
            glColor3f(0,0,0);
            glutSolidSphere(0.005,30,30);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        GLfloat mat_emission6[] = {0.0,1.0,0.0,0.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission6);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
        glTranslatef(posicionsbolles[6][0],posicionsbolles[6][1],posicionsbolles[6][2]);
        glBegin(GL_POLYGON);
            glColor3f(0,1,0);
            glutSolidSphere(0.005,30,30);
        glEnd();
    glPopMatrix();
}

void Display(void){

    glClearColor(0.0,0.0,0.0,0.0);
    init();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(posicio_camera_x, posicio_camera_y, posicio_camera_z, posicio_apunta_x, posicio_apunta_y, posicio_apunta_z, 0.0, 1.0, 0.0);

    glEnable(GL_TEXTURE_2D);

    crearParedes();
    pintarSostre();
    pintarQuadre();
    glDisable(GL_LIGHTING);
    pintarLlum();
    pintarLLumQuadre();
    glEnable(GL_LIGHTING);
    
    pintarColumna();

    if(!fusio){
        fisio();
    }
    
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(posicio_apunta_x,posicio_apunta_y,posicio_apunta_z);
    glColor3f(0.0,0.0,0.0);
    glutSolidSphere(0.001,30.0,30.0);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
	glFlush();
}

void eventoVentana(GLsizei ancho, GLsizei alto)
{

    glViewport(0, 0, ancho, alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // compara aquí amb ortho i frustrum
	gluPerspective(45.0f,(GLdouble) ancho/alto , 0.0, 10.0);

    //actualitzar la posició de les llums aquí
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
        }else if(torn ==2){
            alto_textura = ALTO_TEXTURA_SOSTRE;
            ancho_textura = ANCHO_TEXTURA_SOSTRE;
        }else if(torn ==3){
            alto_textura = ALTO_TEXTURA_MARIE;
            ancho_textura = ANCHO_TEXTURA_MARIE;
        }else if(torn ==4){
            alto_textura = ALTO_TEXTURA_METAL;
            ancho_textura = ANCHO_TEXTURA_METAL;
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
                }else if(torn ==2){
                 textura_sostre[j][i][0] = (GLubyte)r;
                 textura_sostre[j][i][1] = (GLubyte)g;
                 textura_sostre[j][i][2] = (GLubyte)b; 
                }else if(torn ==3){
                 textura_marie[j][i][0] = (GLubyte)r;
                 textura_marie[j][i][1] = (GLubyte)g;
                 textura_marie[j][i][2] = (GLubyte)b; 
                }else if(torn ==4){
                 textura_metal[j][i][0] = (GLubyte)r;
                 textura_metal[j][i][1] = (GLubyte)g;
                 textura_metal[j][i][2] = (GLubyte)b;
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
  char nom2 [] = "sostre.tga";
  leeTextura(nom2,2);
  char nom3 [] = "marie.tga";
  leeTextura(nom3,3);
  char nom4 [] = "metal.tga";
  leeTextura(nom4,4);
  /* Definici�n de los par�metros iniciales de texturacion */ 
  glBindTexture(GL_TEXTURE_2D, nombreTexturas[0]); 
  
  GLfloat tparams[]={0,1.4,0,0.5};
  GLfloat sparams[]={1,0.0,0.0,0.5};

  
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

  glBindTexture(GL_TEXTURE_2D, nombreTexturas[2]);
  
  glTexImage2D(GL_TEXTURE_2D, 0, 3, 600, 600, 
	       0, GL_RGB, GL_UNSIGNED_BYTE, textura_sostre);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, nombreTexturas[3]);
  
  glTexImage2D(GL_TEXTURE_2D, 0, 3, ANCHO_TEXTURA_MARIE, ALTO_TEXTURA_MARIE, 
	       0, GL_RGB, GL_UNSIGNED_BYTE, textura_marie);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, nombreTexturas[4]);
  
  glTexImage2D(GL_TEXTURE_2D, 0, 3, ANCHO_TEXTURA_METAL, ALTO_TEXTURA_METAL, 
	       0, GL_RGB, GL_UNSIGNED_BYTE, textura_metal);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGenfv(GL_S, GL_OBJECT_PLANE, sparams);
  glTexGenfv(GL_T, GL_OBJECT_PLANE, tparams);

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

void estic_dedins(float provisional_x,float provisional_z){
    //Parets
    dedins = true;

    if((provisional_x<-0.8)||(provisional_x>0.8)||(provisional_z > -0.20)||(provisional_z < -4.8)){
        dedins = false;
    }else{
        if((provisional_x>-0.23)&&(posicio_camera_x<0.23)){
            if((provisional_z < -2.0)&&(provisional_z>-3)){
                dedins = false;
            }
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

    float provisional_x = 0.0;
    float provisional_z = 0.0;
    
    switch(cas){
        case 1:
        provisional_x = posicio_camera_x+0.05*posicio_x;
        provisional_z = posicio_camera_z+0.05*posicio_z;

        estic_dedins(provisional_x,provisional_z);

        if(dedins){
            posicio_camera_x += 0.05*posicio_x;
            posicio_camera_z += 0.05*posicio_z;
        }
        break;
        case 2:
        provisional_x = posicio_camera_x-0.05*posicio_x;
        provisional_z = posicio_camera_z-0.05*posicio_z;
        

        estic_dedins(provisional_x,provisional_z);

        if(dedins){
            posicio_camera_x -= 0.05*posicio_x;
            posicio_camera_z -= 0.05*posicio_z;
        }

        break;
        case 3:
        provisional_x = posicio_camera_x-0.05*posicio_x;
        provisional_z = posicio_camera_z-0.05*posicio_z;

        estic_dedins(provisional_x,provisional_z);

        if(dedins){
            posicio_camera_x -= 0.05*posicio_x;
            posicio_camera_z -= 0.05*posicio_z;
        }
        break;
        case 4:
        provisional_x = posicio_camera_x+0.05*posicio_x;
        provisional_z = posicio_camera_z+0.05*posicio_z;

        estic_dedins(provisional_x,provisional_z);

        if(dedins){
            posicio_camera_x += 0.05*posicio_x;
            posicio_camera_z += 0.05*posicio_z;
        }
        break;
    }
        tornar_lloc();


}

void experiment(void){
        if(posicionsbolles[0][0] < posicionsbolles[1][0]){
            posicionsbolles[0][0] += 0.0001;
        }else{
            if(posicionsbolles[5][2] > -2.69){
                posicionsbolles[1][0] += 0.0001;
                posicionsbolles[1][2] -= 0.0001;
                posicionsbolles[2][0] += 0.0001;
                posicionsbolles[2][2] -= 0.0001;
                posicionsbolles[5][0] += 0.0001;
                posicionsbolles[5][2] -= 0.0001;

                posicionsbolles[3][0] += 0.0001;
                posicionsbolles[3][2] += 0.0001;
                posicionsbolles[4][0] += 0.0001;
                posicionsbolles[4][2] += 0.0001;
                posicionsbolles[6][0] += 0.0001;
                posicionsbolles[6][2] += 0.0001;


            }
        }
        glutPostRedisplay();

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
        case 'p':
                if(angle1_llum < 90){
                    angle1_llum += 0.5;
                    angle2_llum += 0.5;
                    altura_llums += 0.5/180;
                }
				break;
        case 'o':
                if(angle1_llum > 0){
                    angle1_llum -= 0.5;
                    angle2_llum -= 0.5;
                    altura_llums -= 0.5/180;
                }
				break;
        case 'k':
                ences_esquerre = !ences_esquerre;
                break;
        case 'l':
                ences_dreta = !ences_dreta;
                break;
        case 'j':
                ences_quadre = !ences_quadre;
                break;
        case 'm' :
                if(!fusio){
                 glutIdleFunc(experiment);
                }
                break;
        case 'n' :
                if(!fusio){
                 glutIdleFunc(NULL);
                }
                break;
	}
	glutPostRedisplay();
}


void opcionesVisualizacion(void)
{
    glutCreateMenu(menuapp);
        glutAddMenuEntry("Fusio", 1);
        glutAddMenuEntry("Fissio", 2);
        glutAddMenuEntry("Salir", 3);
        glutAttachMenu(GLUT_RIGHT_BUTTON);

    printf(" flecha superior - enfocar la càmera cap a dalt\n");
    printf(" flecha inferior - enfocar la càmera cap a baix\n");
    printf("flecha esquerre - enfocar la càmera cap a l'esquerre\n");
    printf("flecha dreta - enfocar la càmera cap a la dreta\n");
    printf("p - puja bombilles\n");
    printf("o - baixa bombilles\n");
    printf("k - apaga/encen llum esquerre\n");
    printf("l - apaga/encen llum dreta\n");
    printf("j- apaga/encen llum quadre\n");
    printf("m - comença experiment\n");
    printf("n - acaba experiment\n");

}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow("Part6");


    glutReshapeFunc(eventoVentana);
	glutDisplayFunc(Display);

	glutSpecialFunc(ProcessSpecialKeys);
	glutKeyboardFunc(ProcessNormalKeys);


    opcionesVisualizacion();
    material();
    init();

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}


