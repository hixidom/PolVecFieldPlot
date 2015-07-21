//Polar vector field plotter, by Andrew Barrette.
//Simple program that plots a 2D polar function of the form {rho(rho,phi), phi(rho,phi)}.
//Function must be defined in the code below.

//g++ -o vectorplot vectorplot.cpp glut32.lib -lopengl32 -lglu32  //use this format to compile
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include "GL/glut.h"
#include <math.h>
#include <vector>
using namespace std;

#define PI 3.14159265359

GLint winw=750,winh=750;
float INTERVAL=.123,PARAM1=1,PARAM2=1;
int STREAMLENGTH=3,STREAMNUM=12,plottype=0;
GLfloat zoomscale=1,xoffset=0,yoffset=0;
char current_key='\0';

float dist(float *r){
    return sqrt(r[0]*r[0]+r[1]*r[1]);
}

float* func(float x,float y){
    float rho=x*x+y*y; //define polar coordinate rho
    float phi=atan(y/x); //define polar coordinate phi
    
    float vpol[2];
    GLfloat *vcart=new GLfloat[2];
    
    //DEFINE FUNCTION HERE!
    vpol[0]=cos(phi)*((2.0/pow(rho,3))+PARAM1); //rho component of vector; DEFINE FUNCTION HERE
    vpol[1]=-sin(phi)*((1.0/pow(rho,2))-(rho*PARAM1)); //phi component of vector; DEFINE FUNCTION HERE
    
    vcart[0]=(vpol[0]*cos(phi))+vpol[1]*cos(phi+(PI/2)); //xcomponent of vector
    vcart[1]=(vpol[0]*sin(phi))+vpol[1]*sin(phi+(PI/2)); //ycomponent of vector
    float scale=.7*INTERVAL/sqrt(pow(vcart[0],2)+pow(vcart[1],2));
    vcart[0]=scale*vcart[0]+x;
    vcart[1]=scale*vcart[1]+y;
    
    return vcart;
}

void vectorplot(){
    float i,j;
    for(i=-STREAMNUM*INTERVAL/2;i<STREAMNUM*INTERVAL/2;i+=INTERVAL)
        for(j=-STREAMNUM*INTERVAL/2;j<STREAMNUM*INTERVAL/2;j+=INTERVAL){
            GLfloat *vectorend=func(i,j);
            glBegin(GL_LINES);
                glVertex2f(i,j);
                glVertex2fv(vectorend);
            glEnd();
            glPointSize(3);
            glBegin(GL_POINTS);
                glVertex2f(i,j);
            glEnd();
            delete(vectorend);
        }
}

void streamplot(){
    float i,j;
    GLfloat r[2];
    for(j=-1;j<1;j+=2.0/STREAMNUM){
        r[0]=sqrt(pow(PARAM2,2)-pow(j,2)); r[1]=j;
        glBegin(GL_LINE_STRIP);
        glVertex2fv(r);
        for(i=0;i<STREAMLENGTH/INTERVAL;i++){
            GLfloat *vectorend=func(r[0],r[1]);
            r[0]=vectorend[0];
            r[1]=vectorend[1];
            delete(vectorend);
            
            glVertex2fv(r);
        }
        glEnd();
    }
}

void resize(int x,int y){
    if(x>0)winw=x;
    if(y>0)winh=y;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,1,1,-1);
}

void disp() 
{
    glClear(GL_COLOR_BUFFER_BIT);		     // Clear Screen and Depth Buffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(xoffset,yoffset,0);
    glScalef(zoomscale,zoomscale,zoomscale);
    
    switch(plottype){
        case 0:vectorplot();break;
        case 1:streamplot();break;
    }
    glutSwapBuffers();
}

void init() 
{
    srand(time(NULL));
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0,0,winw,winh);
	glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,1,1,-1);
}

void keyfunc(unsigned char key, int x, int y){
    switch(key){
        case '=':zoomscale*=1.4; break;
        case '-':zoomscale/=1.4; break;
        case 'p':plottype=(plottype+1)%2;
    }
    current_key=key;
}

void specialkeyfunc(int key, int x, int y){
    if(current_key){
        float direc;
        if(key==GLUT_KEY_UP)direc=1;
        else if(key==GLUT_KEY_DOWN)direc=-1;
        else direc=0;
        
        switch(current_key){
            case '1': PARAM1*=pow(1.1,direc); break;
            case '2': PARAM2*=pow(1.1,direc); break;
            case 'i': INTERVAL*=pow(1.1,direc); break;
            case 'l': STREAMLENGTH+=direc; break;
            case 'n': STREAMNUM+=direc; break;
        }
    }
}
    
void mouse(int but,int state,int x,int y){
    if(but==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
    }
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);  // Display Mode
	glutInitWindowSize(winw,winh);					// set window size
	glutInitWindowPosition(0,0);
	glutCreateWindow("Polar vector field plotter");				// create Window
	glutDisplayFunc(disp);									// register Display Function
	glutIdleFunc(disp);						
    glutMouseFunc(mouse);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyfunc);
	glutSpecialFunc(specialkeyfunc);
	init();
	glutMainLoop();												// run GLUT mainloop
	return 0;
}
