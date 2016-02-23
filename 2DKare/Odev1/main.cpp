#include <stdio.h>
#include <stdlib.h> 
#include <stdarg.h>
#include <math.h>
#include <cstring>
 

#include "glut.h"

void drawString (char *s, float x, float y);
void ifDontCrush(GLint* point,GLint rotation);
void dragTimer(int v);
void isCompletedTimer(int v);

const GLint rectCount = 4;
GLint rectActive=0; 

GLint *point_x;
GLint *point_y;

GLint mouse_x;
GLint mouse_y;

GLfloat windowWidth;
GLfloat windowHeight;

GLint rectSize;
GLint step = 1;
GLboolean rectDrag = false;
GLboolean isCompleted = false;

char *title;
void *font;

void display(){
 
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  if(isCompleted){
		glColor3f(1.0f, 1.0f, 1.0f);
		for(int i=0;i<rectCount;i++)
			glRectf(point_x[i], point_y[i],point_x[i]+rectSize, point_y[i]+rectSize);
  }else{

	  for(int i=0;i<rectCount;i++){
		if(point_x[i]<1)
			point_x[i]=0;
		else if(point_x[i]+rectSize >= windowWidth)
			point_x[i] = windowWidth - rectSize;

		if(point_y[i]<1)
			point_y[i]=0;
  		else if(point_y[i]+rectSize >= windowHeight)
			point_y[i] = windowHeight - rectSize;
	  }

	  for(int i=0;i<rectCount;i++){

		  switch(i){
			case 0:
				glColor3f(1.0f, 0.0f, 0.0f);
				title = "F1";
			break;
			case 1:
				glColor3f(0.0f, 1.0f, 0.0f);
				title = "F2";
				break;	
			case 2:
				glColor3f(0.0f, 0.0f, 1.0f);
				title = "F3";
				break;
			case 3:
				glColor3f(1.0f, 1.0f, 0.0f);
				title = "F4";
			break;
			default:
				glColor3f(0.0f, 0.0f, 0.0f);
				title = "F";
			break;
		  }
			 
		glRectf(point_x[i], point_y[i],point_x[i]+rectSize, point_y[i]+rectSize);
		
		glColor3f(0.0f, 0.0f, 0.0f);
		if(i == rectActive){
			drawString("[", point_x[i]+1, point_y[i]+6);
			drawString("]", point_x[i]+20, point_y[i]+6);
		}
		drawString(title, point_x[i]+5, point_y[i]+5);
	  }
  }

  glutSwapBuffers();
}

 
void specialKeys( int key, int x, int y ) {
	if(isCompleted) return;
	if (key == GLUT_KEY_F1)
		rectActive = 0;
	else if(key == GLUT_KEY_F2)
		rectActive = 1;
	else if(key == GLUT_KEY_F3)
		rectActive = 2;
	else if(key == GLUT_KEY_F4)
		rectActive = 3;

	if (key == GLUT_KEY_RIGHT)
		ifDontCrush(&point_x[rectActive],step);

	else if (key == GLUT_KEY_LEFT)
		ifDontCrush(&point_x[rectActive],-step);

	if (key == GLUT_KEY_UP)
		ifDontCrush(&point_y[rectActive],step);

	else if (key == GLUT_KEY_DOWN)
		ifDontCrush(&point_y[rectActive],-step);


 // printf("%d>%.0f,%.0f\n",rectActive, point_x[rectActive],point_y[rectActive]);

  glutPostRedisplay();
 
}

void mouse(int mouse, int state, int x, int y){
	if(isCompleted) return;
    switch(mouse){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){

				 for(int i=0;i<rectCount;i++){
						 // printf("%d>%.0f,%.0f\n",i, point_x[i],point_y[i]);
						if(x >= point_x[i] && x <= point_x[i] + rectSize &&
							windowHeight - y >= point_y[i] && windowHeight - y <= point_y[i] + rectSize){
								rectActive = i;
								glutPostRedisplay();
								rectDrag = true;
								glutTimerFunc(1, dragTimer, 1);
							return;
						}
				 }

			}else{
				rectDrag = false;
			}
        break;
    }
}

void drag(int x, int y){
if(rectDrag == false) return;

	mouse_x = x;
	mouse_y = y;
}

void isCompletedTimer(int v){

	//printf("%d > %d,%d	%d,%d	%d,%d	%d,%d\n",(int)isCompleted,point_x[0],point_y[0],point_x[1],point_y[1],point_x[2],point_y[2],point_x[3],point_y[3]);

	isCompleted = false;
	for(int i=0;i<rectCount;i++){

		int j;
		for(j=0;j<rectCount;j++){ // sag
			if(j==i) continue;
			if(point_x[i] == point_x[j] + rectSize && point_y[i] == point_y[j]){
				j = rectCount;
			}
		}

		int k;
		for(k=0;k<rectCount;k++){ // alt
			if(k==i || k==j) continue;
			if(point_x[i] == point_x[k] && point_y[i] == point_y[k]  + rectSize){
				k = rectCount;
			}
		}

		int l;
		for(l=0;l<rectCount;l++){ // sag alt
			if(l==i || l==j || l==k) continue;
			if(point_x[i] == point_x[l]  + rectSize && point_y[i] == point_y[l]  + rectSize){
				l = rectCount;
			}
		}

		//printf("%d  %d	%d	%d\n",i,j,k,l);
		if(j == rectCount + 1 && k == rectCount + 1 && l == rectCount + 1){
			isCompleted = true;
			rectActive = i;
			glutPostRedisplay();
			return;
		}
	}

	glutTimerFunc(500, isCompletedTimer, v);  //repost timer 
}


void dragTimer(int v){
		if(point_x[rectActive]< mouse_x - rectSize / 2)
			ifDontCrush(&point_x[rectActive],step);
		else if(point_x[rectActive] >  mouse_x - rectSize / 2)
			ifDontCrush(&point_x[rectActive],-step);

		if(point_y[rectActive] < (windowHeight - mouse_y) - rectSize / 2)
			ifDontCrush(&point_y[rectActive],step);
		else if(point_y[rectActive] > (windowHeight - mouse_y) - rectSize / 2)
			ifDontCrush(&point_y[rectActive],-step);

		glutPostRedisplay();

		if(rectDrag)
			glutTimerFunc(1, dragTimer, v);  //repost timer 
}


void ifDontCrush(GLint* point,GLint rotation){
	if(isCompleted) return;

	*point=*point+rotation;
  	for(int i=0;i<rectCount;i++){
		if(i==rectActive) continue;
		
		if(point_x[rectActive] + rectSize> point_x[i] && point_x[rectActive]< point_x[i] + rectSize  &&
			point_y[rectActive]  < point_y[i] + rectSize 
			&& point_y[rectActive] > point_y[i] -  rectSize
			){
				*point=*point-rotation;
				//printf("%d > true\n",i);
				return;
		}else {
			//printf("%d > false\n",i);
		}
	}
	
}


void changeSize(GLsizei w, GLsizei h)
	{

    glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	windowWidth = w;
	windowHeight = h;

	rectSize = windowWidth/10;

	 printf("windowWidth:%.0f,	windowHeight:%.0f\n\r",windowWidth,windowHeight);

	glOrtho(0.0f, windowWidth, 0.0f, windowHeight, 1.0f, -1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	 glutPostRedisplay();
	}

void drawString (char *s, float x, float y){
     unsigned int i;
	 glRasterPos2i(x, y);

     for (i = 0; i < strlen (s); i++)
		 glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, s[i]);
}

void initialize(){
	windowWidth = 500.0f;
	windowHeight = 500.0f;

	point_x = (GLint*)malloc(sizeof(GLint)*rectCount); 
	point_y = (GLint*)malloc(sizeof(GLint)*rectCount); 

	point_x[0]=0;
	point_y[0]=0;

	point_x[1]=0;
	point_y[1]=windowHeight - rectSize;

	point_x[2]=windowWidth - rectSize;
	point_y[2]=windowHeight - rectSize;

	point_x[3]=windowWidth - rectSize;
	point_y[3]=0;

	glutInitWindowSize(windowWidth,windowHeight);
}

int main(int argc, char* argv[]){

  initialize();

	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutCreateWindow("Ödev 1");

	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutSpecialFunc(specialKeys);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	glutTimerFunc(500, isCompletedTimer, 1);

	glutMainLoop();
 
  return 0;
 
}

