//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  FILE NAME: RailwayWorld.cpp
//  See Lab02.pdf for details
//  ========================================================================

#include <math.h>
#include <GL/freeglut.h>
#include "ass_Models.cpp"
#include "ass_Models.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/freeglut.h>
#include "loadTGA.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#define GL_CLAMP_TO_EDGE 0x812F //Include this line only if GL_CLAMP_TO_EDGE is undefined

using namespace std;
int icurr = 0;
int option = 2;         //View modes:  1 = walking view, 2 = room view
float angle=0, look_x = 180, look_y=0, look_z=-270, eye_x = 0, eye_y = 15, eye_z = 80;  //Camera parameters
const int NPTS = 492;    //Number of points on the path
GLuint txId[20];   //Texture ids
float ptx[NPTS], ptz[NPTS];
float  inner_track = 5;
float  outer_track = 6;
GLUquadric *q;
int delay = 0;

void loadPath()
{
    ifstream ifile;
    ifile.open("Oval.txt");
    for (int i = 0; i < NPTS; i++)
        ifile >> ptx[i] >> ptz[i];

    ifile.close();
}
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
void drawPath()
{
    glColor3f(0.0, 0.0, 1.0);
    glDisable(GL_LIGHTING);


///////////////////      Track Segments     /////////////////////


    glBegin(GL_QUADS);     //// outer track segment
        for (int i = 0; i < NPTS-2; i++) {

            glm::vec3 P (ptx[i],0.1, ptz[i]); //P
            glm::vec3 Q (ptx[i + 1],0.1, ptz[i + 1]); //Q

            glm::vec3 P2 (ptx[i + 1],0.1, ptz[i + 1]); //P+1
            glm::vec3 Q2 (ptx[i + 2],0.1, ptz[i + 2]); //Q+1

            glm::vec3 u = Q - P ; // the direction at the current position
            glm::vec3 u_norm = glm::normalize(u); //normallize of V

            glm::vec3 u2 = Q2 - P2 ; // the direction at the next position
            glm::vec3 u_norm2 = glm::normalize(u2); //normallize of V2

            glm::vec3 v (u_norm[2],1, -1*u_norm[0]);
            glm::vec3 v2 (u_norm2[2],1, -1*u_norm2[0]);

            glVertex3f(ptx[i] + inner_track*v[0], 1, ptz[i] + inner_track*v[2]);
            glVertex3f(ptx[i] + outer_track*v[0], 1, ptz[i] + outer_track*v[2]);
            glVertex3f(ptx[i + 1] + outer_track*v2[0], 1, ptz[i + 1] + outer_track*v2[2]);
            glVertex3f(ptx[i + 1] + inner_track*v2[0], 1, ptz[i + 1] + inner_track*v2[2]);

        }
            glm::vec3 P (ptx[491],0.1, ptz[491]); //P
            glm::vec3 Q (ptx[0],0.1, ptz[0]); //Q

            glm::vec3 P2 (ptx[0],0.1, ptz[0]); //P+1
            glm::vec3 Q2 (ptx[1],0.1, ptz[1]); //Q+1

            glm::vec3 u = Q - P ; // the direction at the current position
            glm::vec3 u_norm = glm::normalize(u); //normallize of V

            glm::vec3 u2 = Q2 - P2 ; // the direction at the next position
            glm::vec3 u_norm2 = glm::normalize(u2); //normallize of V2

            glm::vec3 v (u_norm[2],0.1, -1*u_norm[0]);
            glm::vec3 v2 (u_norm2[2],0.1, -1*u_norm2[0]);
            glVertex3f(ptx[491] + inner_track*v[0], 1, ptz[491] + inner_track*v[2]);
            glVertex3f(ptx[491] + outer_track*v[0], 1, ptz[491] + outer_track*v[2]);
            glVertex3f(ptx[0] + outer_track*v2[0], 1, ptz[0] + outer_track*v2[2]);
            glVertex3f(ptx[0] + inner_track*v2[0], 1, ptz[0] + inner_track*v2[2]);

            glVertex3f(ptx[490] + inner_track*v[0], 1, ptz[490] + inner_track*v[2]);
            glVertex3f(ptx[490] + outer_track*v[0], 1, ptz[490] + outer_track*v[2]);
            glVertex3f(ptx[491] + outer_track*v2[0], 1, ptz[491] + outer_track*v2[2]);
            glVertex3f(ptx[491] + inner_track*v2[0], 1, ptz[491] + inner_track*v2[2]);


    glEnd();

    glBegin(GL_QUADS);     //// from outer track segment
        for (int i = 0; i < NPTS-2; i++) {

            glm::vec3 P (ptx[i],0.1, ptz[i]); //P
            glm::vec3 Q (ptx[i + 1],0.1, ptz[i + 1]); //Q

            glm::vec3 P2 (ptx[i + 1],0.1, ptz[i + 1]); //P+1
            glm::vec3 Q2 (ptx[i + 2],0.1, ptz[i + 2]); //Q+1

            glm::vec3 u = Q - P ; // the direction at the current position
            glm::vec3 u_norm = glm::normalize(u); //normallize of V

            glm::vec3 u2 = Q2 - P2 ; // the direction at the next position
            glm::vec3 u_norm2 = glm::normalize(u2); //normallize of V2

            glm::vec3 v (u_norm[2],1, -1*u_norm[0]);
            glm::vec3 v2 (u_norm2[2],1, -1*u_norm2[0]);

            glVertex3f(ptx[i] + outer_track*v[0], 0.1, ptz[i] + outer_track*v[2]);
            glVertex3f(ptx[i] + outer_track*v[0], 1, ptz[i] + outer_track*v[2]);
            glVertex3f(ptx[i + 1] + outer_track*v2[0], 1, ptz[i + 1] + outer_track*v2[2]);
            glVertex3f(ptx[i + 1] + outer_track*v2[0], 0.1, ptz[i + 1] + outer_track*v2[2]);

        }

            glVertex3f(ptx[491] + outer_track*v[0], 0.1, ptz[491] + outer_track*v[2]);
            glVertex3f(ptx[491] + outer_track*v[0], 1, ptz[491] + outer_track*v[2]);
            glVertex3f(ptx[0] + outer_track*v2[0], 1, ptz[0] + outer_track*v2[2]);
            glVertex3f(ptx[0] + outer_track*v2[0], 0.1, ptz[0] + outer_track*v2[2]);

            glVertex3f(ptx[490] + outer_track*v[0], 0.1, ptz[490] + outer_track*v[2]);
            glVertex3f(ptx[490] + outer_track*v[0], 1, ptz[490] + outer_track*v[2]);
            glVertex3f(ptx[491] + outer_track*v2[0], 1, ptz[491] + outer_track*v2[2]);
            glVertex3f(ptx[491] + outer_track*v2[0], 0.1, ptz[491] + outer_track*v2[2]);


    glEnd();

    glBegin(GL_QUADS);     ////inner outer track segment
        for (int i = 0; i < NPTS-2; i++) {

            glm::vec3 P (ptx[i],0.1, ptz[i]); //P
            glm::vec3 Q (ptx[i + 1],0.1, ptz[i + 1]); //Q

            glm::vec3 P2 (ptx[i + 1],0.1, ptz[i + 1]); //P+1
            glm::vec3 Q2 (ptx[i + 2],0.1, ptz[i + 2]); //Q+1

            glm::vec3 u = Q - P ; // the direction at the current position
            glm::vec3 u_norm = glm::normalize(u); //normallize of V

            glm::vec3 u2 = Q2 - P2 ; // the direction at the next position
            glm::vec3 u_norm2 = glm::normalize(u2); //normallize of V2

            glm::vec3 v (u_norm[2],1, -1*u_norm[0]);
            glm::vec3 v2 (u_norm2[2],1, -1*u_norm2[0]);

            glVertex3f(ptx[i] + inner_track*v[0], 0.1, ptz[i] + inner_track*v[2]);
            glVertex3f(ptx[i] + inner_track*v[0], 1, ptz[i] + inner_track*v[2]);
            glVertex3f(ptx[i + 1] + inner_track*v2[0], 1, ptz[i + 1] + inner_track*v2[2]);
            glVertex3f(ptx[i + 1] + inner_track*v2[0], 0.1, ptz[i + 1] + inner_track*v2[2]);

        }

            glVertex3f(ptx[491] + inner_track*v[0], 0.1, ptz[491] + inner_track*v[2]);
            glVertex3f(ptx[491] + inner_track*v[0], 1, ptz[491] + inner_track*v[2]);
            glVertex3f(ptx[0] + inner_track*v2[0], 1, ptz[0] + inner_track*v2[2]);
            glVertex3f(ptx[0] + inner_track*v2[0], 0.1, ptz[0] + inner_track*v2[2]);

            glVertex3f(ptx[490] + inner_track*v[0], 0.1, ptz[490] + inner_track*v[2]);
            glVertex3f(ptx[490] + inner_track*v[0], 1, ptz[490] + inner_track*v[2]);
            glVertex3f(ptx[491] + inner_track*v2[0], 1, ptz[491] + inner_track*v2[2]);
            glVertex3f(ptx[491] + inner_track*v2[0], 0.1, ptz[491] + inner_track*v2[2]);


    glEnd();


    glBegin(GL_QUADS);     //// inner track segment
        for (int i = 0; i < NPTS-2; i++) {

            glm::vec3 P (ptx[i],0.1, ptz[i]); //P
            glm::vec3 Q (ptx[i + 1],0.1, ptz[i + 1]); //Q

            glm::vec3 P2 (ptx[i + 1],0.1, ptz[i + 1]); //P+1
            glm::vec3 Q2 (ptx[i + 2],0.1, ptz[i + 2]); //Q+1

            glm::vec3 u = Q - P ; // the direction at the current position
            glm::vec3 u_norm = glm::normalize(u); //normallize of V

            glm::vec3 u2 = Q2 - P2 ; // the direction at the next position
            glm::vec3 u_norm2 = glm::normalize(u2); //normallize of V2

            glm::vec3 v (-1*u_norm[2],0.1, 1*u_norm[0]);
            glm::vec3 v2 (-1*u_norm2[2],0.1, 1*u_norm2[0]);

            glVertex3f(ptx[i] + inner_track*v[0], 1, ptz[i] + inner_track*v[2]);
            glVertex3f(ptx[i] + outer_track*v[0], 1, ptz[i] + outer_track*v[2]);
            glVertex3f(ptx[i + 1] + outer_track*v2[0], 1, ptz[i + 1] + outer_track*v2[2]);
            glVertex3f(ptx[i + 1] + inner_track*v2[0], 1, ptz[i + 1] + inner_track*v2[2]);
        }

            glm::vec3 v1 (-1*u_norm[2],0.1, 1*u_norm[0]);
            glm::vec3 v3 (-1*u_norm2[2],0.1, 1*u_norm2[0]);
            glVertex3f(ptx[491] + inner_track*v1[0], 1, ptz[491] + inner_track*v1[2]);
            glVertex3f(ptx[491] + outer_track*v1[0], 1, ptz[491] + outer_track*v1[2]);
            glVertex3f(ptx[0] + outer_track*v3[0], 1, ptz[0] + outer_track*v3[2]);
            glVertex3f(ptx[0] + inner_track*v3[0], 1, ptz[0] + inner_track*v3[2]);

            glVertex3f(ptx[490] + inner_track*v1[0], 1, ptz[490] + inner_track*v1[2]);
            glVertex3f(ptx[490] + outer_track*v1[0], 1, ptz[490] + outer_track*v1[2]);
            glVertex3f(ptx[491] + outer_track*v3[0], 1, ptz[491] + outer_track*v3[2]);
            glVertex3f(ptx[491] + inner_track*v3[0], 1, ptz[491] + inner_track*v3[2]);

    glEnd();

    glBegin(GL_QUADS);     //// from inner track segment
        for (int i = 0; i < NPTS-2; i++) {

            glm::vec3 P (ptx[i],0.1, ptz[i]); //P
            glm::vec3 Q (ptx[i + 1],0.1, ptz[i + 1]); //Q

            glm::vec3 P2 (ptx[i + 1],0.1, ptz[i + 1]); //P+1
            glm::vec3 Q2 (ptx[i + 2],0.1, ptz[i + 2]); //Q+1

            glm::vec3 u = Q - P ; // the direction at the current position
            glm::vec3 u_norm = glm::normalize(u); //normallize of V

            glm::vec3 u2 = Q2 - P2 ; // the direction at the next position
            glm::vec3 u_norm2 = glm::normalize(u2); //normallize of V2

            glm::vec3 v (-1*u_norm[2],0.1, 1*u_norm[0]);
            glm::vec3 v2 (-1*u_norm2[2],0.1, 1*u_norm2[0]);

            glVertex3f(ptx[i] + outer_track*v[0], 0.1, ptz[i] + outer_track*v[2]);
            glVertex3f(ptx[i] + outer_track*v[0], 1, ptz[i] + outer_track*v[2]);
            glVertex3f(ptx[i + 1] + outer_track*v2[0], 1, ptz[i + 1] + outer_track*v2[2]);
            glVertex3f(ptx[i + 1] + outer_track*v2[0], 0.1, ptz[i + 1] + outer_track*v2[2]);
        }


            glVertex3f(ptx[491] + outer_track*v1[0], 0.1, ptz[491] + outer_track*v1[2]);
            glVertex3f(ptx[491] + outer_track*v1[0], 1, ptz[491] + outer_track*v1[2]);
            glVertex3f(ptx[0] + outer_track*v3[0], 1, ptz[0] + outer_track*v3[2]);
            glVertex3f(ptx[0] + outer_track*v3[0], 0.1, ptz[0] + outer_track*v3[2]);

            glVertex3f(ptx[490] + outer_track*v1[0], 0.1, ptz[490] + outer_track*v1[2]);
            glVertex3f(ptx[490] + outer_track*v1[0], 1, ptz[490] + outer_track*v1[2]);
            glVertex3f(ptx[491] + outer_track*v3[0], 1, ptz[491] + outer_track*v3[2]);
            glVertex3f(ptx[491] + outer_track*v3[0], 0.1, ptz[491] + outer_track*v3[2]);

    glEnd();

    glBegin(GL_QUADS);     //// inner track segment
        for (int i = 0; i < NPTS-2; i++) {

            glm::vec3 P (ptx[i],0.1, ptz[i]); //P
            glm::vec3 Q (ptx[i + 1],0.1, ptz[i + 1]); //Q

            glm::vec3 P2 (ptx[i + 1],0.1, ptz[i + 1]); //P+1
            glm::vec3 Q2 (ptx[i + 2],0.1, ptz[i + 2]); //Q+1

            glm::vec3 u = Q - P ; // the direction at the current position
            glm::vec3 u_norm = glm::normalize(u); //normallize of V

            glm::vec3 u2 = Q2 - P2 ; // the direction at the next position
            glm::vec3 u_norm2 = glm::normalize(u2); //normallize of V2

            glm::vec3 v (-1*u_norm[2],0.1, 1*u_norm[0]);
            glm::vec3 v2 (-1*u_norm2[2],0.1, 1*u_norm2[0]);

            glVertex3f(ptx[i] + inner_track*v[0], 0.1, ptz[i] + inner_track*v[2]);
            glVertex3f(ptx[i] + inner_track*v[0], 1, ptz[i] + inner_track*v[2]);
            glVertex3f(ptx[i + 1] + inner_track*v2[0], 1, ptz[i + 1] + inner_track*v2[2]);
            glVertex3f(ptx[i + 1] + inner_track*v2[0], 0.1, ptz[i + 1] + inner_track*v2[2]);
        }


            glVertex3f(ptx[491] + inner_track*v1[0], 0.1, ptz[491] + inner_track*v1[2]);
            glVertex3f(ptx[491] + inner_track*v1[0], 1, ptz[491] + inner_track*v1[2]);
            glVertex3f(ptx[0] + inner_track*v3[0], 1, ptz[0] + inner_track*v3[2]);
            glVertex3f(ptx[0] + inner_track*v3[0], 0.1, ptz[0] + inner_track*v3[2]);

            glVertex3f(ptx[490] + inner_track*v1[0], 0.1, ptz[490] + inner_track*v1[2]);
            glVertex3f(ptx[490] + inner_track*v1[0], 1, ptz[490] + inner_track*v1[2]);
            glVertex3f(ptx[491] + inner_track*v3[0], 1, ptz[491] + inner_track*v3[2]);
            glVertex3f(ptx[491] + inner_track*v3[0], 0.1, ptz[491] + inner_track*v3[2]);

    glEnd();


///////////////////      Railway Sleepers     ////////////////
    for (int i = 0; i < NPTS - 2; i += 4) {
        float s1 = 9;
        float s2 = 2;
        glColor3f(1, 0, 0);
        glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glm::vec3 P (ptx[i],0.1, ptz[i]); //P
            glm::vec3 Q (ptx[i + 1],0.1, ptz[i + 1]); //Q

            glm::vec3 u = Q - P ; // the direction at the current position
            glm::vec3 u_norm = glm::normalize(u); //normallize of V
            glm::vec3 v (u_norm[2],0.1, -1*u_norm[0]);
            glVertex3f(ptx[i] - s1*v[0], 0.1, ptz[i] - s1*v[2]);
            glVertex3f(ptx[i] + s1*v[0], 0.1, ptz[i] + s1*v[2]);
            glVertex3f(ptx[i] + s1*v[0] + s2 * u_norm[0], 0.1, ptz[i] + s1*v[2]+ s2 * u_norm[2]);
            glVertex3f(ptx[i] - s1*v[0] + s2 * u_norm[0], 0.1, ptz[i] - s1*v[2]+ s2 * u_norm[2]);

        glEnd();

    }


}

void loadTexture()
{
    glGenTextures(30, txId);     // Create 2 texture ids
///////////// sky box  /////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    loadTGA("L1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //front
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    loadTGA("F1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //right
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadTGA("R1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //back
    glBindTexture(GL_TEXTURE_2D, txId[3]);
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

    glBindTexture(GL_TEXTURE_2D, txId[5]);
    loadTGA("D1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

///////////// train  //////////////////////

    glBindTexture(GL_TEXTURE_2D, txId[6]);
    loadTGA("wagon.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


    glBindTexture(GL_TEXTURE_2D, txId[7]);
    loadTGA("back_train.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


    glBindTexture(GL_TEXTURE_2D, txId[8]);
    loadTGA("head.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    glBindTexture(GL_TEXTURE_2D, txId[9]);
    loadTGA("left_train.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


    glBindTexture(GL_TEXTURE_2D, txId[10]);
    loadTGA("right.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


    glBindTexture(GL_TEXTURE_2D, txId[11]);
    loadTGA("train_top.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

//////////////// ground ////////////////

    glBindTexture(GL_TEXTURE_2D, txId[12]);
    loadTGA("snow.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[13]);
    loadTGA("roof.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[14]);
    loadTGA("ground.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[15]);
    loadTGA("wall.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[16]);
    loadTGA("tun.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[17]);
    loadTGA("tun2.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}
void floor()
{

    glColor4f(1, 1, 1, 1.0);  //The floor is gray in colour
    glNormal3f(0.0, 1.0, 0.0);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, txId[14]);
    //We can suppress specular reflections from the floor by temporarily setting the specular colour to black.


     //The floor is made up of several tiny squares on a 400 x 400 grid. Each square has a unit size.
    glBegin(GL_QUADS);
    for(int i = -400; i < 400; i+=5)
    {
        for(int j = -400;  j < 400; j+=5)
        {
            glTexCoord2f(0., 0.);
            glVertex3f(i, 0, j);
            glTexCoord2f(0., 1.);
            glVertex3f(i, 0, j+5);
            glTexCoord2f(1., 1.);
            glVertex3f(i+5, 0, j+5);
            glTexCoord2f(1., 0.);
            glVertex3f(i+5, 0, j);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

}
void skybox()
{
    glColor3f(1., 1., 1.);
    glEnable(GL_TEXTURE_2D);
    // ////////////////////// LEFT WALL ///////////////////////
    float x = 200,y1 = -200, y2 = 200,z = 200;
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);     glVertex3f(-x,  y1, z);
        glTexCoord2f(1, 0);   glVertex3f(-x, y1, -z);
        glTexCoord2f(1, 1);    glVertex3f(-x, y2, -z);
        glTexCoord2f(0, 1);    glVertex3f(-x, y2, z);
    glEnd();

    ////////////////////// FRONT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[1]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-x,  y1, -z);
        glTexCoord2f(1, 0); glVertex3f(x, y1, -z);
        glTexCoord2f(1, 1); glVertex3f(x, y2, -z);
        glTexCoord2f(0, 1); glVertex3f(-x,  y2, -z);
    glEnd();

    ////////////////////// RIGHT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[0]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(x,  y1, -z);
        glTexCoord2f(1, 0); glVertex3f(x, y1, z);
        glTexCoord2f(1, 1); glVertex3f(x, y2,  z);
        glTexCoord2f(0, 1); glVertex3f(x,  y2,  -z);
    glEnd();


    ////////////////////// back WALL ////////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[3]);
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

    glBindTexture(GL_TEXTURE_2D, txId[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(x, y1, z);
        glTexCoord2f(1, 0); glVertex3f(-x, y1,  z);
        glTexCoord2f(1, 1); glVertex3f(-x, y1,  -z);
        glTexCoord2f(0, 1); glVertex3f(x, y1, -z);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

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
void tunnel()
{
    int y = 40, x = 40, z = 20;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[16]);
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0., 1.);
    glVertex3f(-x, y, -z);
    glTexCoord2f(0, 0);
    glVertex3f(-x, 0, -z);
    glTexCoord2f(1, 1);
    glVertex3f(x, y, -z);
    glTexCoord2f(1, 0);
    glVertex3f(x, 0, -z);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0., 1.);
    glVertex3f(-x, y, z);
    glTexCoord2f(0, 0);
    glVertex3f(-x, 0, z);
    glTexCoord2f(1, 1);
    glVertex3f(x, y, z);
    glTexCoord2f(1, 0);
    glVertex3f(x, 0, z);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, txId[17]);
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0., 1.);
    glVertex3f(-x, y, z);
    glTexCoord2f(0, 0);
    glVertex3f(-x, y, -z);
    glTexCoord2f(1, 1);
    glVertex3f(x, y, z);
    glTexCoord2f(1, 0);
    glVertex3f(x, y, -z);
    glEnd();


    glDisable(GL_TEXTURE_2D);


}
void station()
{
    ////////// station ground /////////
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glScalef(30.0, 8.0, 110.0);
    glColor3f(0.34, 0.31, 0.31);
    glutSolidCube(1.0);


    glColor3f(0.16, 0.37, 0.58);
//////////////// zhu zi ////////////
    for (int i = 0; i < 6; i++){
        glPushMatrix();
        glTranslatef(50 - 20*i, 12, 0);
        glRotatef(90, 0, 1, 0);
        glScalef(5.0, 60.0, 5.0);
        glutSolidCube(1.0);
        glPopMatrix();
    }

///////////////// top ////////////////////
    glPushMatrix();
    glTranslatef(0, 40, 0);
    glRotatef(90, 0, 1, 0);
    glScalef(30.0, 8.0, 110.0);
    glColor3f(0.34, 0.31, 0.31);
    glutSolidCube(1.0);
    glPopMatrix();


}
void station_floor()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[15]);


    glBegin(GL_QUADS);
    int s = 4;
    for(int i = 0; i < 110; i+=s)
    {
        for(int j = 0;  j < 30; j+=s)
        {
            glTexCoord2f(0., 0.);
            glVertex3f(i, 4, j);
            glTexCoord2f(0., 1.);
            glVertex3f(i, 4, j+s);
            glTexCoord2f(1., 1.);
            glVertex3f(i+s, 4, j+s);
            glTexCoord2f(1., 0.);
            glVertex3f(i+s, 4, j);
        }
    }
    glEnd();
        glDisable(GL_TEXTURE_2D);
}
void rof()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[13]);


    glBegin(GL_QUADS);
    int s = 25;
    for(int i = 0; i < 100; i+=s)
    {
        for(int j = 0;  j < 30; j+=s)
        {
            glTexCoord2f(0., 0.);
            glVertex3f(i, 4, j);
            glTexCoord2f(0., 1.);
            glVertex3f(i, 4, j+s);
            glTexCoord2f(1., 1.);
            glVertex3f(i+s, 4, j+s);
            glTexCoord2f(1., 0.);
            glVertex3f(i+s, 4, j);
        }
    }
    glEnd();
        glDisable(GL_TEXTURE_2D);
}
void train ()
{
    float x = 10.01, y = 15, z = 5;
    glPushMatrix();
        glRotatef(180, 0, 1, 0);
        base();
    glPopMatrix();
    //glColor3f(0, 1, 1);
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, txId[8]);

    ////////// from//////////
    glBegin(GL_QUADS);
    //glTexCoord2f(1, 0);
    glVertex3f(-x,5,-z);
    //glTexCoord2f(0, 0);
    glVertex3f(-x,5,z);
    //glTexCoord2f(0, 1);
    glVertex3f(-x,y,z);
    //glTexCoord2f(1, 1);
    glVertex3f(-x,y,-z);
    glEnd();
    /////////////  right
    glBindTexture(GL_TEXTURE_2D, txId[10]);
    glBegin(GL_QUADS);
    //glTexCoord2f(1, 0);
    glVertex3f(-x,5,z);
    //glTexCoord2f(0, 0);
    glVertex3f(x,5,z);
    //glTexCoord2f(0, 1);
    glVertex3f(x,y,z);
    //glTexCoord2f(1, 1);
    glVertex3f(-x,y,z);
    glEnd();
    ///////////// left
    glBegin(GL_QUADS);
    glVertex3f(-x,5,-z);
    glVertex3f(x,5,-z);
    glVertex3f(x,y,-z);
    glVertex3f(-x,y,-z);
    glEnd();

    ////////// back//////////
    glBegin(GL_QUADS);
    //glTexCoord2f(0, 0);
    glVertex3f(x,5,-z);
    //glTexCoord2f(0, 1);
    glVertex3f(x,5,z);
    //glTexCoord2f(1, 1);
    glVertex3f(x,y,z);
    //glTexCoord2f(1, 0);
    glVertex3f(x,y,-z);
    glEnd();

    ////////// top//////////
    glBegin(GL_QUADS);
    glVertex3f(-x,y,-z);
    glVertex3f(x,y,-z);
    glVertex3f(x,y,z);
    glVertex3f(-x,y,z);
    glEnd();


    //glDisable(GL_TEXTURE_2D);


}
void myTimer(int value)
{

    if (icurr == 40 && delay <300) {
        delay++;
    } else {
            icurr++;
            delay = 0;

    }

    //icurr = icurr + 1;
    glutPostRedisplay();

    glutTimerFunc(5, myTimer, 0);
}

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
void display(void)
{


   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

/////////////////////////////      cam       /////////////////////////
    float lookx, lookz;
    if (option == 1)
    {
        gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0, 1, 0);
    } else if(option == 2) {
        gluLookAt(12., 13, -70.0, 13., 12., -50., 0, 1., 0);
    } else if (option == 3) {
        if (icurr != NPTS-1) {
            lookx = ptx[icurr+5] , lookz = ptz[icurr+5];
        } else {
            lookx = ptx[0], lookz = ptz[0];
        }
        gluLookAt(ptx[icurr+1], 10, ptz[icurr+1], lookx, 10., lookz, 0., 1., 0.);
    }


   drawPath();
   floor();
   skybox();
    //axises();
    //train();


   glPushMatrix();
    glTranslatef(-30,0,-70);
    station();
   glPopMatrix();


   glPushMatrix();
    glTranslatef(-85,0.5,-85);
    station_floor();
   glPopMatrix();

   glPushMatrix();
    glTranslatef(0,0,40);
    tunnel();
   glPopMatrix();


   glPushMatrix();
    glTranslatef(-80,31,-100);
    rof();
   glPopMatrix();

////////////////      loco     ////////////////
    glm::vec3 u0 (1,0,0);

    if (icurr == NPTS-1){
        icurr = 0;
    }
    glm::vec3 P (ptx[icurr], 1, ptz[icurr]); //P
    glm::vec3 Q (ptx[icurr + 1], 1, ptz[icurr + 1]); //Q
    glm::vec3 V = Q - P ; // the direction at the current position
    glm::vec3 u = glm::normalize(V); //normallize of V
    float angle = glm::dot(u, u0);
    //float angle = atan2(u[2], -1*u[0]);
    float angle_deg = 180*acos(angle)/M_PI;
    //glm::vec3 cprod = glm::cross(u0, u);
    glPushMatrix();
        //printf("%f \n", angle_deg);
        glTranslatef(ptx[icurr], 1, ptz[icurr]);

        if(ptx[icurr]<-60){
            glRotatef(angle_deg, 0, -1, 0);
        }else{
            glRotatef(angle_deg, 0, 1, 0);
        }

        glRotatef(180, 0, 1, 0);
        engine();
    glPopMatrix();

    int index;
//////////////////       wagons      //////////////////////
    for (int i = 1; i < 6; i++)
    {
        index = icurr - 20*i;
        glm::vec3 u0 (1,0,0);

        if (index < 0)
        {
            index = 491 + index;
        }
        glm::vec3 P (ptx[index], 1, ptz[index]); //P
        glm::vec3 Q (ptx[index + 1], 1, ptz[index + 1]); //Q
        glm::vec3 V = Q - P ; // the direction at the current position
        glm::vec3 u = glm::normalize(V); //normallize of V
        float angle = glm::dot(u, u0);
        //float angle = atan2(u[2], -1*u[0]);
        float angle_deg = 180*acos(angle)/M_PI;


        glPushMatrix();
            glTranslatef(ptx[index], 1, ptz[index]);
            if(ptx[index]<-60){
                glRotatef(angle_deg, 0, -1, 0);
            }else{
                glRotatef(angle_deg, 0, 1, 0);
            }
            glRotatef(180, 0, 1, 0);
            wagon();
        glPopMatrix();
    }



   glutSwapBuffers();   //Useful for animation
}


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
    if (eye_x >= 185){
        eye_x = 184;
        } else if (eye_x <= -185) {
        eye_x = -184;
    }
    if (eye_z >= 185) {
        eye_z = 184;
        } else if (eye_z <= -185) {
        eye_z = -184;
    }
    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == '1') option = 1;
    else if (key == '2') option = 2;
    else if (key == '3') option = 3;
    glutPostRedisplay();
}


//---------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize (1200, 1200);
   glutInitWindowPosition (450, 450);
   glutCreateWindow ("Ass1_11969443");
   initialize ();
   glutTimerFunc(5, myTimer, 0);

   glutSpecialFunc(special);
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}
