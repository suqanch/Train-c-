//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//  Menghao Zhan   11708670
//  Subject       < Snowpiercer >
//  ========================================================================
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "loadTGA.h"
GLUquadric *q;
using namespace std;

float angle=0, look_x = 180, look_y=0, look_z=-270, eye_x = 0, eye_y = 10, eye_z = 0;  //Camera parameters
int delay = 0;

const int NPTS = 817;
float ptx[NPTS], ptz[NPTS];
int option = 0;

int pos = 50;
glm::vec3 u(-1,0,0);

//Reads flight path data from Ltrack.txt
void loadPath()
{
    ifstream ifile;
    ifile.open("Ltrack.txt");
    for (int i = 0; i < NPTS; i++)
        ifile >> ptx[i] >> ptz[i];
    ifile.close();
}

//--------------------------------------------------------------------------------
GLuint txId[2];   //Texture ids
float thx[] = {1.5,1.2,0.975364759,0.743883277,0.540460157,0.379124579,0.217789001,0.077497194,-0.041750842,-0.146969697,-0.238159371,-0.301290685,-0.350392817,-0.427553311,-0.455611672,-0.497699214,-0.525757576,-0.497699214,-0.41352413,-0.287261504,0};
float thy[] = {10,9.7,9.390504888,9.058379284,8.658750956,8.292060704,7.807952309,7.390635014,6.956238716,6.404220953,5.885547911,5.299372143,4.830411197,4.227664731,3.607432619,3.02095187,2.266527595,1.477741993,0.922979391,0.451476927,0};
//--------------------------------------------------------------------------------
void loadTexture()
{
    glGenTextures(15, txId);     // Create texture ids

    glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadTGA("L1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //front
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    loadTGA("F1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //right
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    loadTGA("R1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //back
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    loadTGA("B1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // top
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    loadTGA("U1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[13]);
    loadTGA("D1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[5]);
    loadTGA("snowfloor.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[6]);
    loadTGA("trainhead.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[7]);
    loadTGA("trainside.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[8]);
    loadTGA("wagon.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[9]);
    loadTGA("top.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[10]);
    loadTGA("wall.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[11]);
    loadTGA("tunnel.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[12]);
    loadTGA("roof.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

}

//-------------------------------------------------------------------

void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 )
{
    float nx, ny, nz;
    nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
    ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
    nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

    glNormal3f(nx, ny, nz);
}
//--------------------------------------------------------------------------------

void skybox()
{

    glColor3f(1., 1., 1.);
    glEnable(GL_TEXTURE_2D);
    // ////////////////////// LEFT WALL ///////////////////////
    float x = 200,y1 = -200, y2 = 200,z = 200;
    glBindTexture(GL_TEXTURE_2D, txId[3]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);     glVertex3f(-x,  y1, z);
        glTexCoord2f(1, 0);   glVertex3f(-x, y1, -z);
        glTexCoord2f(1, 1);    glVertex3f(-x, y2, -z);
        glTexCoord2f(0, 1);    glVertex3f(-x, y2, z);
    glEnd();

    ////////////////////// FRONT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[0]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-x,  y1, -z);
        glTexCoord2f(1, 0); glVertex3f(x, y1, -z);
        glTexCoord2f(1, 1); glVertex3f(x, y2, -z);
        glTexCoord2f(0, 1); glVertex3f(-x,  y2, -z);
    glEnd();

    ////////////////////// RIGHT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[2]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(x,  y1, -z);
        glTexCoord2f(1, 0); glVertex3f(x, y1, z);
        glTexCoord2f(1, 1); glVertex3f(x, y2,  z);
        glTexCoord2f(0, 1); glVertex3f(x,  y2,  -z);
    glEnd();


    ////////////////////// REAR WALL ////////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[1]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( x, y1, z);
        glTexCoord2f(1, 0); glVertex3f(-x, y1,  z);
        glTexCoord2f(1, 1); glVertex3f(-x, y2,  z);
        glTexCoord2f(0, 1); glVertex3f( x, y2, z);
    glEnd();

    /////////////////////// TOP //////////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[4]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(x, y2, z);
        glTexCoord2f(1, 0); glVertex3f(-x, y2,  z);
        glTexCoord2f(1, 1); glVertex3f(-x, y2,  -z);
        glTexCoord2f(0, 1); glVertex3f(x, y2, -z);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, txId[13]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(x, y1, z);
        glTexCoord2f(1, 0); glVertex3f(-x, y1,  z);
        glTexCoord2f(1, 1); glVertex3f(-x, y1,  -z);
        glTexCoord2f(0, 1); glVertex3f(x, y1, -z);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}
//--------------------------------------------------------------------------------
void floor()
{
    float white[4] = {1., 1., 1., 1.};
    float black[4] = {0};
    glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
    glNormal3f(0.0, 1.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
    glBindTexture(GL_TEXTURE_2D, txId[5]);

    int s = 4;
    //The floor is made up of several tiny squares on a 400 x 400 grid. Each square has a unit size.
    glBegin(GL_QUADS);
    for(int i = -400; i < 400; i+=s)
    {
        for(int j = -400;  j < 400; j+=s)
        {
            glTexCoord2f(0., 0.);
            glVertex3f(i, 0, j);
            glTexCoord2f(0., 1.);
            glVertex3f(i, 0, j+s);
            glTexCoord2f(1., 1.);
            glVertex3f(i+s, 0, j+s);
            glTexCoord2f(1., 0.);
            glVertex3f(i+s, 0, j);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}
//--------------- MODEL BASE --------------------------------------
void wheels()
{
    glPushMatrix();
        glColor4f(0.96, 0.86, 0.27, 1.0);             //Connector between wagons
        glTranslatef(14.0, 8.0, 0.0);
        glScalef(5.0, 5.0, 3);
        glutSolidCube(2.0);
    glPopMatrix();

    //8 Wheels (radius = 2 units)
    //x, z positions of wheels:
    float wx[8] = {  2.5, 2.5 , 6.5, 6.5, 10.5, 10.5, 14.5, 14.5  };
    float wz[8] = { -5, 5, -5, 5, -5, 5, -5, 5 };



    for (int i = 0; i < 8; i++)
    {
        GLUquadric *q = gluNewQuadric();   //Disc
        glPushMatrix();
        glColor4f(0.21, 0.23, 0.52, 1.0);    //Wheel color
        glTranslatef(wx[i]+1, 0.5, wz[i]);
        glRotatef(10*pos, 0, 0, 1);
        gluDisk(q, 0.0, 2, 30, 2);
        // glutSolidTorus(0, 2, 30, 30);

        glPushMatrix();
        glColor4f(0.77, 0.78, 0.85, 1);
        glScalef(1.0, 1.0, 0.1);
        glutSolidCube(2.5);
        glPopMatrix();

        glPopMatrix();
    }
}

//-----------------------------------------------------------

void axises()
{

                   //Front

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 50; i++){
        glColor3f(1, 1, 0);
        glVertex3f(i, 0, 0); }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 50; i++){
        glColor3f(1, 1, 0);
        glVertex3f(0, i, 0);}
        glEnd();

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 50; i++){
        glColor3f(1, 0, 1);
        glVertex3f(0, 0, i);}
        glEnd();


}


void train_head()
{
    float xt = 2, yt = 1.5, l = 17;

//Front
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glColor3f(1, 1, 1);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < 21; i++)
    {
        normal(thx[i]*xt, thy[i]*yt, -5,
            thx[i]*xt, thy[i]*yt, -5,
            thx[i]*xt, thy[i]*yt, 5);
        glTexCoord2f(0.2, (float)i/(float)(21-1));
        glVertex3f(thx[i]*xt, thy[i]*yt, -5);
        glTexCoord2f(0.8 , (float)i/(float)(21-1));
        glVertex3f(thx[i]*xt, thy[i]*yt, 5);
    }
    glEnd();
//Back
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0, 1);
    glVertex3f(l,0,7);
    glTexCoord2f(1, 1);
    glVertex3f(l,0,-7);
    glTexCoord2f(0, 0);
    glVertex3f(l,15,7);
    glTexCoord2f(1, 0);
    glVertex3f(l,15,-7);

    glEnd();

    glBegin(GL_QUAD_STRIP);                  //Left-front
    for (int i = 0; i < 21; i++)
    {
        normal(thx[i]*xt, thy[i]*yt, 5,
            thx[i]*xt+3, thy[i]*yt, 5,
            thx[i]*xt+3, thy[i]*yt, 7);
        glTexCoord2f(0.8, (float)i/(float)(21-1));
        glVertex3f(thx[i]*xt, thy[i]*yt, 5);
        glTexCoord2f(1. , (float)i/(float)(21-1));
        glVertex3f(thx[i]*xt+3, thy[i]*yt, 7);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, txId[7]);
    glBegin(GL_QUAD_STRIP);                  //Left-body
    for (int i = 0; i < 21; i++)
    {
        normal(thx[i]*xt+3, thy[i]*yt, 7,
            thx[i]*xt+3, thy[i]*yt, 7,
            thx[i]*xt+l, thy[i]*yt, 7);
        glTexCoord2f(0.2, (float)i/(float)(21-1));
        glVertex3f(thx[i]*xt+3, thy[i]*yt, 7);
        glTexCoord2f(1, (float)i/(float)(21-1));
        glVertex3f(l, thy[i]*yt, 7);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUAD_STRIP);                  //Right-front
    for (int i = 0; i < 21; i++)
    {
        normal(thx[i]*xt, thy[i]*yt, -5,
            thx[i]*xt+3, thy[i]*yt, -5,
            thx[i]*xt+3, thy[i]*yt, -7);
        glTexCoord2f(0., (float)i/(float)(21-1));
        glVertex3f(thx[i]*xt+3, thy[i]*yt, -7);
        glTexCoord2f(0.2, (float)i/(float)(21-1));
        glVertex3f(thx[i]*xt, thy[i]*yt, -5);

    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, txId[7]);
    glBegin(GL_QUAD_STRIP);                  //Right-body
    for (int i = 0; i < 21; i++)
    {
        normal(thx[i]*xt+3, thy[i]*yt, -7,
            thx[i]*xt+3, thy[i]*yt, -7,
            thx[i]*xt+l, thy[i]*yt, -7);

        glTexCoord2f(0.2, (float)i/(float)(21-1));
        glVertex3f(thx[i]*xt+3, thy[i]*yt, -7);
        glTexCoord2f(1, (float)i/(float)(21-1));
        glVertex3f(l, thy[i]*yt, -7);
    }
    glEnd();
//Top
    float topx[] = {6,3,3,6};
    float topz[]= {-7,-5,5,7};
    glBindTexture(GL_TEXTURE_2D, txId[9]);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < 4; i++)
    {
    glTexCoord2f(0., (float)i/(float)(4-1));
    glVertex3f(topx[i], thy[0]*yt, topz[i]);
    glTexCoord2f(0., (float)i/(float)(4-1));
    glVertex3f(l, thy[0]*yt, topz[i]);
    }
    glEnd();
//Bottom
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < 4; i++)
    {
    glVertex3f(topx[i], 2, topz[i]);
    glVertex3f(l, 2, topz[i]);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    wheels();
}

//--------------- WAGON ----------------------------------

void wagon()
{
    float t = 0.3;
    glEnable(GL_TEXTURE_2D);
    //RIGHT
    glBindTexture(GL_TEXTURE_2D, txId[8]);
    glColor3f(1, 1, 1);
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0, 1);
    glVertex3f(17, 0, -7);
    glTexCoord2f(t, 1);
    glVertex3f(0, 0, -7);
    glTexCoord2f(0, 0);
    glVertex3f(17, 15, -7);
    glTexCoord2f(t, 0);
    glVertex3f(0, 15, -7);
    glEnd();

    //LEFT
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0, 1);
    glVertex3f(17, 0, 7);
    glTexCoord2f(t, 1);
    glVertex3f(0, 0, 7);
    glTexCoord2f(0, 0);
    glVertex3f(17, 15, 7);
    glTexCoord2f(t, 0);
    glVertex3f(0, 15, 7);
    glEnd();

    //FRONT
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0, 1);
    glVertex3f(0, 0, -7);
    glTexCoord2f(t, 1);
    glVertex3f(0, 0, 7);
    glTexCoord2f(0, 0);
    glVertex3f(0, 15, -7);
    glTexCoord2f(t, 0);
    glVertex3f(0, 15, 7);
    glEnd();

    //BACK
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0, 1);
    glVertex3f(17, 0, -7);
    glTexCoord2f(t, 1);
    glVertex3f(17, 0, 7);
    glTexCoord2f(0, 0);
    glVertex3f(17, 15, -7);
    glTexCoord2f(t, 0);
    glVertex3f(17, 15, 7);
    glEnd();
    //TOP
    glBindTexture(GL_TEXTURE_2D, txId[9]);
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0, 1);
    glVertex3f(0, 15, -7);
    glTexCoord2f(0, 0);
    glVertex3f(0, 15, 7);
    glTexCoord2f(1, 1);
    glVertex3f(17, 15, -7);
    glTexCoord2f(1, 0);
    glVertex3f(17, 15, 7);
    glEnd();
    //BOTTOM
    glDisable(GL_TEXTURE_2D);
    wheels();
}
//--------------- STATION ----------------------------------
// This simple model of a stadion
//--------------------------------------------------------
void station()
{
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glTranslatef(-20, 0, -85.0);
    glPushMatrix();
    glTranslatef(20, 2.49, 80.0);
    glScalef(40.0, 5.0, 160.0);
    glColor4f(0.27, 0.28, 0.26, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();

    float h = 5, r = 30;

    glColor4f(0.27, 0.28, 0.26, 1.0);
    glNormal3f(0.0, 1.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[10]);

    int s = 10;
//Top
    glBegin(GL_QUADS);
    for(int i = 0; i < 40; i+=s)
    {
        for(int j = 0;  j < 160; j+=s)
        {
            glTexCoord2f(0., 0.);
            glVertex3f(i, h, j);
            glTexCoord2f(0., 1.);
            glVertex3f(i, h, j+s);
            glTexCoord2f(1., 1.);
            glVertex3f(i+s, h, j+s);
            glTexCoord2f(1., 0.);
            glVertex3f(i+s, h, j);
        }
    }
    glEnd();
//roof
    glBindTexture(GL_TEXTURE_2D, txId[12]);
    glBegin(GL_QUAD_STRIP);
        for(int j = 0;  j < 160; j++)
        {
            glTexCoord2f((float)j/(float)(160), 0);
            glVertex3f(0, r, j);
            glTexCoord2f((float)j/(float)(160), 1);
            glVertex3f(40, r, j);
        }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, txId[12]);
    glBegin(GL_QUAD_STRIP);
        for(int j = 0;  j < 160; j++)
        {
            glTexCoord2f((float)j/(float)(160), 0);
            glVertex3f(-10, r-5, j);
            glTexCoord2f((float)j/(float)(160), 1);
            glVertex3f(50, r-5, j);
        }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, txId[12]);
    glBegin(GL_QUAD_STRIP);
        for(int j = 0;  j < 160; j++)
        {
            glTexCoord2f((float)j/(float)(160), 0);
            glVertex3f(50, r-5, j);
            glTexCoord2f((float)j/(float)(160), 1);
            glVertex3f(40, r, j);
        }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, txId[12]);
    glBegin(GL_QUAD_STRIP);
        for(int j = 0;  j < 160; j++)
        {
            glTexCoord2f((float)j/(float)(160), 0);
            glVertex3f(-10, r-5, j);
            glTexCoord2f((float)j/(float)(160), 1);
            glVertex3f(0, r, j);
        }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, txId[12]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-10, r-5, 0);
        glTexCoord2f(.5, 0);
        glVertex3f(50, r-5, 0);
        glTexCoord2f(.4, 1);
        glVertex3f(40, r, 0);
        glTexCoord2f(0.1, 1);
        glVertex3f(0, r, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, txId[12]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-10, r-5, 159);
        glTexCoord2f(.5, 0);
        glVertex3f(50, r-5, 159);
        glTexCoord2f(.4, 1);
        glVertex3f(40, r, 159);
        glTexCoord2f(0.1, 1);
        glVertex3f(0, r, 159);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    //columns on the station
    for (int x = -4; x < 5; x+=2) {

        glPushMatrix();
        glTranslatef(-15*x, 20., 0.0);
        glScalef(5.0, 30.0, 5.0);
        glutSolidCube(1.0);
        glPopMatrix();
        }
}
//--------Newtrack-------------------------------------------------------------
void track()
{

    glColor3f(0.06, 0.12, 0.22);
    float w1=4, w2=6, h=1, s1= 8 , s2 = 2;
    float u1x,u1y, u1z;
//Iutside-Top
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < NPTS; i++)
    {
        u1x = ptx[(i+1)%NPTS]-ptx[i%NPTS]; u1z = ptz[(i+1)%NPTS]-ptz[i%NPTS];
        glm::vec3 u1(u1x, 0, u1z);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glVertex3f(ptx[i]+vt1[0]*w1, h, ptz[i]+vt1[2]*w1);
        glVertex3f(ptx[i]+vt1[0]*w2, h, ptz[i]+vt1[2]*w2);
    }
    glEnd();

    glPushMatrix();
    glTranslatef(0, 0.5, -35.0);
    glScalef(4.0, 1.0, 2.0);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0.5, -45.0);
    glScalef(4.0, 1.0, 2.0);
    glutSolidCube(1.0);
    glPopMatrix();
//Iutside-OutSide
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < NPTS-1; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glVertex3f(ptx[i]+vt1[0]*w2, h, ptz[i]+vt1[2]*w2);
        glNormal3s(vt1[0],vt1[1],vt1[2]);
        glVertex3f(ptx[i]+vt1[0]*w2, 0, ptz[i]+vt1[2]*w2);
    }

    glEnd();
//Iutside-InSide
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < NPTS-1; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glVertex3f(ptx[i]+vt1[0]*w1, h, ptz[i]+vt1[2]*w1);
        glNormal3s(vt1[0],vt1[1],vt1[2]);
        glVertex3f(ptx[i]+vt1[0]*w1, 0, ptz[i]+vt1[2]*w1);
    }
    glEnd();
//Onside-Top
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < NPTS-1; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glVertex3f(ptx[i]-vt1[0]*w2, h, ptz[i]-vt1[2]*w2);
        glNormal3s(0,1,0);
        glVertex3f(ptx[i]-vt1[0]*w1, h, ptz[i]-vt1[2]*w1);
    }
    glEnd();
//Onside-OutSide
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < NPTS-1; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glVertex3f(ptx[i]-vt1[0]*w2, h, ptz[i]-vt1[2]*w2);
        glNormal3s(-vt1[0],-vt1[1],-vt1[2]);
        glVertex3f(ptx[i]-vt1[0]*w2, 0, ptz[i]-vt1[2]*w2);
    }
    glEnd();
//Onside-InSide
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < NPTS-1; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glVertex3f(ptx[i]-vt1[0]*w1, h, ptz[i]-vt1[2]*w1);
        glNormal3s(-vt1[0],-vt1[1],-vt1[2]);
        glVertex3f(ptx[i]-vt1[0]*w1, 0, ptz[i]-vt1[2]*w1);
    }
    glEnd();
//Bounds
    for (int i = 0; i < NPTS-1; i += 5 )
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glColor3f(0.41, 0.27, 0.06);
        glBegin(GL_QUADS);
            glNormal3s(0,1,0);
            glVertex3f(ptx[i]+vt1[0]*s1, 0.1, ptz[i]+vt1[2]*s1);
            glVertex3f(ptx[i]+vt1[0]*s1+ut1[0]*s2, 0.1, ptz[i]+vt1[2]*s1+ut1[2]*s2);
            glVertex3f(ptx[i]-vt1[0]*s1+ut1[0]*s2, 0.1, ptz[i]-vt1[2]*s1+ut1[2]*s2);
            glVertex3f(ptx[i]-vt1[0]*s1, 0.1, ptz[i]-vt1[2]*s1);
        glEnd();
    }
}
//-----------------------------------------------------------------------------------
void tunnel()
{

    glColor3f(0.48, 0.47, 0.45);
    glEnable(GL_TEXTURE_2D);
    float W1= 15, w2=10, h=25, s1= 8 , s2 = 2;
    float u1x,u1y, u1z;
//Outside
    glBindTexture(GL_TEXTURE_2D, txId[11]);
    glBegin(GL_QUAD_STRIP);
    for (int i = 480; i < 760; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glTexCoord2f((float)i/(float)(759-480), 1);
        glVertex3f(ptx[i]-vt1[0]*w2, h, ptz[i]-vt1[2]*w2);
        glNormal3s(-vt1[0],-vt1[1],-vt1[2]);
        glTexCoord2f((float)i/(float)(759-480), 0);
        glVertex3f(ptx[i]-vt1[0]*w2, 0, ptz[i]-vt1[2]*w2);
    }
    glEnd();
//Iutside-OutSide
    glBegin(GL_QUAD_STRIP);
    for (int i = 480; i < 760; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glTexCoord2f((float)i/(float)(759-480), 1);
        glVertex3f(ptx[i]+vt1[0]*w2, h, ptz[i]+vt1[2]*w2);
        glNormal3s(vt1[0],vt1[1],vt1[2]);
        glTexCoord2f((float)i/(float)(759-480), 0);
        glVertex3f(ptx[i]+vt1[0]*w2, 0, ptz[i]+vt1[2]*w2);
    }
    glEnd();
//---------------------------------------------------------------------------------------------------
//Outside
    glBegin(GL_QUAD_STRIP);
    for (int i = 480; i < 760; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glTexCoord2f((float)i/(float)(759-480), 1);
        glVertex3f(ptx[i]-vt1[0]*W1, h, ptz[i]-vt1[2]*W1);
        glNormal3s(-vt1[0],-vt1[1],-vt1[2]);
        glTexCoord2f((float)i/(float)(759-480), 0);
        glVertex3f(ptx[i]-vt1[0]*W1, 0, ptz[i]-vt1[2]*W1);
    }
    glEnd();
//Iutside-OutSide
    glBegin(GL_QUAD_STRIP);
    for (int i = 480; i < 760; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glTexCoord2f((float)i/(float)(759-480), 1);
        glVertex3f(ptx[i]+vt1[0]*W1, h, ptz[i]+vt1[2]*W1);
        glNormal3s(vt1[0],vt1[1],vt1[2]);
        glTexCoord2f((float)i/(float)(759-480), 0);
        glVertex3f(ptx[i]+vt1[0]*W1, 0, ptz[i]+vt1[2]*W1);
    }
    glEnd();
//COVER
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0, 0);
    glVertex3f(ptx[480], 0, ptz[480]+W1);
    glTexCoord2f(0.05, 0);
    glVertex3f(ptx[480], 0, ptz[480]+w2);
    glTexCoord2f(0, 1);
    glVertex3f(ptx[480], h, ptz[480]+W1);
    glTexCoord2f(0.05, 1);
    glVertex3f(ptx[480], h, ptz[480]+w2);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0.0, 0);
    glVertex3f(ptx[480], 0, ptz[480]-W1);
    glTexCoord2f(0.05, 0);
    glVertex3f(ptx[480], 0, ptz[480]-w2);
    glTexCoord2f(0.0, 1);
    glVertex3f(ptx[480], h, ptz[480]-W1);
    glTexCoord2f(0.05, 1);
    glVertex3f(ptx[480], h, ptz[480]-w2);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0.0, 0);
    glVertex3f(ptx[759], 0, ptz[760]+W1);
    glTexCoord2f(0.05, 0);
    glVertex3f(ptx[759], 0, ptz[760]+w2);
    glTexCoord2f(0.0, 1);
    glVertex3f(ptx[759], h, ptz[760]+W1);
    glTexCoord2f(0.05, 1);
    glVertex3f(ptx[759], h, ptz[760]+w2);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0.0, 0);
    glVertex3f(ptx[759], 0, ptz[759]-W1);
    glTexCoord2f(0.05, 0);
    glVertex3f(ptx[759], 0, ptz[759]-w2);
    glTexCoord2f(0.0, 1);
    glVertex3f(ptx[759], h, ptz[759]-W1);
    glTexCoord2f(0.05, 1);
    glVertex3f(ptx[759], h, ptz[759]-w2);
    glEnd();

    //Roof
    glBindTexture(GL_TEXTURE_2D, txId[12]);
    glBegin(GL_QUAD_STRIP);
    for (int i = 480; i < 760; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glTexCoord2f((float)i/(float)(759-480), 0);
        glVertex3f(ptx[i]-vt1[0]*W1, h, ptz[i]-vt1[2]*W1);
        glNormal3s(vt1[0],vt1[1],vt1[2]);
        glTexCoord2f((float)i/(float)(759-480), 1);
        glVertex3f(ptx[i]+vt1[0]*W1, h, ptz[i]+vt1[2]*W1);
    }
    glEnd();


    glBegin(GL_QUAD_STRIP);
    for (int i = 480; i < 760; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glTexCoord2f((float)i/(float)(759-480), 0);
        glVertex3f(ptx[i]-vt1[0]*(W1-10), h+5, ptz[i]-vt1[2]*(W1-10));
        glTexCoord2f((float)i/(float)(759-480), 1);
        glVertex3f(ptx[i]+vt1[0]*(W1-10), h+5, ptz[i]+vt1[2]*(W1-10));
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (int i = 480; i < 760; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glTexCoord2f((float)i/(float)(759-480), 0);
        glVertex3f(ptx[i]-vt1[0]*(W1), h, ptz[i]-vt1[2]*(W1));
        glTexCoord2f((float)i/(float)(759-480), 1);
        glVertex3f(ptx[i]+vt1[0]*(W1-20), h+5, ptz[i]+vt1[2]*(W1-20));
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (int i = 480; i < 760; i++)
    {
        glm::vec3 u1(ptx[i+1]-ptx[i], 0, ptz[i+1]-ptz[i]);
        glm::vec3 ut1 = glm::normalize(u1);
        glm::vec3 vt1(ut1[2], 0, -ut1[0]);
        glTexCoord2f((float)i/(float)(759-480), 1);
        glVertex3f(ptx[i]+vt1[0]*(W1), h, ptz[i]+vt1[2]*(W1));
        glNormal3s(vt1[0],vt1[1],vt1[2]);
        glTexCoord2f((float)i/(float)(759-480), 0);
        glVertex3f(ptx[i]+vt1[0]*(W1-10), h+5, ptz[i]+vt1[2]*(W1-10));
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0.0, 0);
    glVertex3f(ptx[480], h, ptz[480]-(W1));
    glTexCoord2f(0.05, 0);
    glVertex3f(ptx[480], h, ptz[480]+(W1));
    glTexCoord2f(0.0, 1);
    glVertex3f(ptx[480], h+5, ptz[480]-(W1-10));
    glTexCoord2f(0.05, 1);
    glVertex3f(ptx[480], h+5, ptz[480]+(W1-10));
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0.0, 0);
    glVertex3f(ptx[759], h, ptz[759]-(W1));
    glTexCoord2f(0.05, 0);
    glVertex3f(ptx[759], h, ptz[759]+(W1));
    glTexCoord2f(0.0, 1);
    glVertex3f(ptx[759], h+5, ptz[759]-(W1-10));
    glTexCoord2f(0.05, 1);
    glVertex3f(ptx[759], h+5, ptz[759]+(W1-10));
    glEnd();
    glDisable(GL_TEXTURE_2D);

}
//--------------------------------------------------------------------------------------------------
void trafficlight_b()
{
    // glColor3f(1, 0.13, 0.31);
    GLUquadric *q = gluNewQuadric();   //Disc

    //lightbox
    glPushMatrix();
    glTranslatef(0,12,0);
    glScalef(1,4,2);
    glColor3f(0.1, 0.11, 0.24);
    glutSolidCube(1);
    glPopMatrix();
    //light row
    glPushMatrix();
    glColor3f(0.23, 0.24, 0.35);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 0.5, 0.5,12,30,30);
    glPopMatrix();
}
//--------------------------------------------------------------------------------------------------
void cross()
{
    for (int i = 10; i < 36; i+=3) {
    glPushMatrix();
    glTranslatef(i,0,0);
    glScalef(2,0.1,20);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
    glPopMatrix();
    }

    for (int i = -10; i > -36; i-=3) {
    glPushMatrix();
    glTranslatef(i,0,0);
    glScalef(2,0.1,20);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
    glPopMatrix();
    }
}
//--------------------------------------------------------------------------------------------------
void trafficlight_l()
{
    GLUquadric *q = gluNewQuadric();

    if (pos >= 70 && pos <= 243) {
        if (pos%26 <= 10) {

        glPushMatrix();
        glTranslatef(0.6,12.85,0);
        glRotatef(-90, 0, 1, 0);
        glColor3f(1, 0.95, 0.);
        gluDisk(q, 0.0, 0.7, 30, 2);
        glPopMatrix();

        //light
        glPushMatrix();
        glTranslatef(-0.6,12.85,0);
        glRotatef(-90, 0, 1, 0);
        glColor3f(1, 0.95, 0.);
        gluDisk(q, 0.0, 0.7, 30, 2);
        glPopMatrix();

            //light
        glPushMatrix();
        glTranslatef(0.6,11.15,0);
        glRotatef(-90, 0, 1, 0);
        glColor3f(1, 0.13, 0.31);
        gluDisk(q, 0.0, 0.7, 30, 2);
        glPopMatrix();

        //light
        glPushMatrix();
        glTranslatef(-0.6,11.15,0);
        glRotatef(-90, 0, 1, 0);
        glColor3f(1, 0.13, 0.31);
        gluDisk(q, 0.0, 0.7, 30, 2);
        glPopMatrix();

        } else {
                        //light
        glPushMatrix();
        glTranslatef(0.6,12.85,0);
        glRotatef(-90, 0, 1, 0);
        glColor3f(1, 0.13, 0.31);
        gluDisk(q, 0.0, 0.7, 30, 2);
        glPopMatrix();

        //light
        glPushMatrix();
        glTranslatef(-0.6,12.85,0);
        glRotatef(-90, 0, 1, 0);
        glColor3f(1, 0.13, 0.31);
        gluDisk(q, 0.0, 0.7, 30, 2);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.6,11.15,0);
        glRotatef(-90, 0, 1, 0);
        glColor3f(1, 0.95, 0);
        gluDisk(q, 0.0, 0.7, 30, 2);
        glPopMatrix();

        //light
        glPushMatrix();
        glTranslatef(-0.6,11.15,0);
        glRotatef(-90, 0, 1, 0);
        glColor3f(1, 0.95, 0);
        gluDisk(q, 0.0, 0.7, 30, 2);
        glPopMatrix();
        }

        //line

    } else {
        //light
    glPushMatrix();
    glTranslatef(0.6,11.15,0);
    glRotatef(-90, 0, 1, 0);
    glColor3f(0.28, 0.28, 0.22);
    gluDisk(q, 0.0, 0.7, 30, 2);
    glPopMatrix();

    //light
    glPushMatrix();
    glTranslatef(-0.6,11.15,0);
    glRotatef(-90, 0, 1, 0);
    glColor3f(0.28, 0.28, 0.22);
    gluDisk(q, 0.0, 0.7, 30, 2);
    glPopMatrix();

    //light
    glPushMatrix();
    glTranslatef(-0.6,12.85,0);
    glRotatef(-90, 0, 1, 0);
    glColor3f(0.28, 0.28, 0.22);
    gluDisk(q, 0.0, 0.7, 30, 2);
    glPopMatrix();

    //light
    glPushMatrix();
    glTranslatef(0.6,12.85,0);
    glRotatef(-90, 0, 1, 0);
    glColor3f(0.28, 0.28, 0.22);
    gluDisk(q, 0.0, 0.7, 30, 2);
    glPopMatrix();
    }

//link
    glPushMatrix();
    glTranslatef(1,5,0);
    glColor3f(1,1,1);
    glScalef(0.8,0.8,0.8);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2,0,0);
    glPushMatrix();
    glTranslatef(0,5,0);
    if (pos >= 70 && pos < 243 ){
        if (1.25*(pos-70) < 75) {
            glRotatef(-1.25*(pos-70), 1, 0, 0);
        } else {
            glRotatef(-75, 1, 0, 0);
        }
    }
    if (pos >= 243) {
        if (1.25*(pos-243) < 75) {
            glRotatef(-75+1.25*(pos-243), 1, 0, 0);
        } else {
            glRotatef(0, 1, 0, 0);
        }
    }
    glRotatef(-15, 1, 0, 0);
    glTranslatef(0,10,0);
    glColor3f(0.69, 0.58, 0);
    glScalef(1,30,1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
void initialize(void)
{
    loadTexture();
    glEnable(GL_TEXTURE_2D);
    glClearColor(0., 1., 1., 1.);    //Background colour
    glEnable(GL_DEPTH_TEST);

    loadPath();

    glEnable(GL_NORMALIZE);
    glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(80., 1, 10.0, 500.0);   //Perspective projection
}
//-------------------------------------------------------------------
//---------------------------------------------------------------------
void display(void)
{

    float lx,lz;
    //train anime
    float xt,zt;
    if (pos != NPTS-1) {
        xt = ptx[pos+1]-ptx[pos] , zt = ptz[pos+1]-ptz[pos];
    } else {
        xt = ptx[0]-ptx[pos] , zt = ptz[0]-ptz[pos];
    }
    glm::vec3 pv(xt, 0, zt);
    glm::vec3 v = glm::normalize(pv);
    glm::vec3 axis = glm::cross(u, v);
    float dprod = glm::dot(u,v);
    //train anime

    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//------Special move of camera--------------------------------------------------------------------------
    if (option == 1) {
        gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0, 1, 0);
    } else if (option == 2) {
        gluLookAt(13., 12, -58.0, 13., 12., -50., 0, 1., 0);
    } else if (option == 3) {
        if (pos != NPTS-1) {
            lx = ptx[pos+1] , lz = ptz[pos+1];
        } else {
            lx = ptx[0], lz = ptz[0];
        }
        gluLookAt(ptx[pos], 10, ptz[pos], lx, 10., lz, 0., 1., 0.);
    } else {
        gluLookAt(-80., 15, -60.0, 300., 15, 150., 0., 1., 0.);
    }
//--------------------------------------------------------------------------------
    skybox();
    floor();
//---------Traffic light--------------------------------------------------------
    glPushMatrix();
    glTranslatef(-90,0,20);
    glScalef(1,1,-1);
    trafficlight_l();
    trafficlight_b();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-90,0,50);
    glScalef(-1,1,1);
    trafficlight_l();
    trafficlight_b();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-110,0,20);
    glScalef(1,1,-1);
    trafficlight_l();
    trafficlight_b();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-110,0,50);
    glScalef(-1,1,1);
    trafficlight_l();
    trafficlight_b();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-100,0,35);
    cross();
    glPopMatrix();
//--------------------------------------------------------------------
    track();
    tunnel();
    // axises();


    glPushMatrix();
    glTranslatef(-20,0,-70);
    station();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(ptx[pos], 2.5, ptz[pos]);
    if (axis[1] >= 0){
        glRotatef(360*acos(dprod)/(2*M_PI), 0, 1, 0);
    } else {
        glRotatef(360*acos(dprod)/(2*M_PI), 0, -1, 0);
    }
    train_head();
    glPopMatrix();

    int wa;
    float xw,zw;
    for (int i = 18; i < 150; i+=18) {

        if (pos-i < 0)
        {
            wa = abs(NPTS+pos-i);
        } else {
            wa = pos-i;
        }
        if (wa != NPTS-1) {
            xw = ptx[wa+1]-ptx[wa] , zw = ptz[wa+1]-ptz[wa];
        } else {
            xw = ptx[wa]-ptx[abs(NPTS-i)], zw = ptz[wa]-ptz[abs(NPTS-i)];
        }
        glm::vec3 pvw(xw, 0, zw);
        glm::vec3 vw = glm::normalize(pvw);
        glm::vec3 axisw = glm::cross(u, vw);
        float dprodw = glm::dot(u,vw);
        glPushMatrix();
        glTranslatef(ptx[wa], 2.5, ptz[wa]);
        if (axisw[1] >= 0){
            glRotatef(360*acos(dprodw)/(2*M_PI), 0, 1, 0);
        } else {
            glRotatef(360*acos(dprodw)/(2*M_PI), 0, -1, 0);
        }
        if (i < 100) {
            wagon();
        } else if (i > 118 && i< 128){
            glScalef(-1,1,1);
            train_head();
        }

        glPopMatrix();
    }

   glutSwapBuffers();   //Useful for animation

}
//---------------------------------------------------------------------

void myTimer (int value)
{

    if (pos == 50 && delay <300) {
        delay++;
        pos == 50;
    } else {
        if  (pos < NPTS-1) {
            pos++;
        } else {
            pos = 0;
            delay = 0;
        }
    }

        glutPostRedisplay();
        value ++;
        glutTimerFunc(2, myTimer, value);

}
//--------------------------------------------------------------------------------
//  Keyboard call-back function
//  Used for selecting a view mode
void keyboard(unsigned char key, int x, int y)
{
    if (key == 'c') option++;
    if (option > 4) option = 0;
    glutPostRedisplay();
}
//---------------------------------------------------------------------
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle -= 0.1;
    else if(key == GLUT_KEY_RIGHT) angle += 0.1;
    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= sin(angle);
        eye_z += cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += sin(angle);
        eye_z -= cos(angle);
    }
    else if(key == GLUT_KEY_PAGE_DOWN)
    { // decrease height
        look_y -= 1;
    }
    else if(key == GLUT_KEY_PAGE_UP)
    { // Increase height
        look_y += 1;
    }
    if (eye_x >= 185) {eye_x = 184;}
    else if (eye_x <= -185) {eye_x = -184;}
    if (eye_z >= 185) {eye_z = 184;}
    else if (eye_z <= -185) {eye_z = -184;}
    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    glutPostRedisplay();
}
//-------------------------------------------------------------------
//---------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize (1080, 1080);
   glutInitWindowPosition (400, 100);
   glutCreateWindow ("Toy Train");
   initialize ();
   glutTimerFunc(100, myTimer, 0);
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
