#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glut.h"
#include <stdio.h>
#include <math.h>

// Rotation amounts
static GLfloat xRot = 90.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;

GLfloat *xPos;
GLfloat *yPos;
GLfloat *zPos;

GLbyte *color;
GLbyte active=0;
static GLbyte count=3;

GLfloat zoom=2.0f;
GLint windowWidth;
GLint windowHeight;

GLint mouseButton;
GLint mouseState;
GLint mouseX;
GLint mouseY;

GLint i;

// Light values and coordinates
GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };

// Called to draw scene
void RenderScene(void)
	{
	// Clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	//--- BEGIN ZOOM ----
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (windowWidth <= windowHeight) 
		 //gluPerspective (50.0*zoomFactor, (float)width/(float)height, zNear, zFar);
		glOrtho (-zoom, zoom, -zoom*windowHeight/windowWidth, zoom*windowHeight/windowWidth, -zoom, zoom);
    else 
		glOrtho (-zoom*windowWidth/windowHeight, zoom*windowWidth/windowHeight, -zoom, zoom, -zoom, zoom);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//--- END ZOOM ----

	glPushMatrix();
	glTranslatef(0.0f,0.0f,0.0f);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f,0.0f,0.0f);
	glColor3ub(255, 255, 0);
    glutSolidCone(0.6f, 1.8f, 50, 50);
    glPopMatrix();


	for(i=0;i<count;i++){
		glPushMatrix();
		glTranslatef(0.0f,0.0f,0.0f);
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		glRotatef(zRot, 0.0f, 0.0f, 1.0f);
		glTranslatef(xPos[i],yPos[i],zPos[i]);

		switch(color[i]){
		case 1:
			glColor3ub(255,0,0);
		break;
		case 2:
			glColor3ub(0,255,0);
		break;
		default:
			glColor3ub(0,0,255);
		}

		glutSolidSphere(0.30, 50, 50);
		glPopMatrix();
	}


	// Flush drawing commands
	glutSwapBuffers();
	}

void SetupRC()
	{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Setup and enable light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glEnable(GL_LIGHT0);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	glMateriali(GL_FRONT,GL_SHININESS,128);

	}

void SpecialKeys(int key, int x, int y)
	{
	switch(key){
	case GLUT_KEY_UP:
		xRot-= 5.0f;
	break;
	case GLUT_KEY_DOWN:
		xRot += 5.0f;
	break;
	case GLUT_KEY_LEFT:
		zRot += 5.0f;
	break;
	case GLUT_KEY_RIGHT:
		zRot -= 5.0f;
	break;
	case GLUT_KEY_F1:
		active=0;
	break;
	case GLUT_KEY_F2:
		active=1;
	break;
	case GLUT_KEY_F3:
		active=2;
	break;
	}

	glutPostRedisplay();
	}


void KeyboardKeys(int key, int x, int y)
	{
	switch (key){
	case 'w':
	case 'W':
		zPos[active] -= 0.1f;
	break;
	case 's':
	case 'S':
		zPos[active] += 0.1f;;
	break;
	case 'a':
	case 'A':
		xPos[active] -= 0.1f;
	break;
	case 'd':
	case 'D':
		xPos[active] += 0.1f;
	break;
	case 'c':
	case 'C':
		yPos[active] += 0.1f;
	break;
	case 'v':
	case 'V':
		yPos[active] -= 0.1f;
	break;
	case '+':
		yRot+= 5.0f;
	break;
	case '-':
		yRot-= 5.0f;
	break;
	case '*':
		if(zoom > 2.0f)
			zoom-= 0.5f;
	break;
	case '/':
		zoom+=  0.5f;
	break;
	}

	//printf("%f\n",zoom);

	glutPostRedisplay();
	}

void mouse(int button, int state, int x, int y) 
{	
	mouseState = state;
	mouseButton = button;
	mouseX = x;
	mouseY = y;
}

void motion(int x,int y){
	if (mouseState == GLUT_DOWN )
		{
			switch (mouseButton){
				case GLUT_LEFT_BUTTON:
					xRot -= (mouseY - y);
					zRot += (mouseX - x);
					break;
				case GLUT_RIGHT_BUTTON:
					if(zoom > 2.0f &&  mouseY > y)
					zoom-= 0.2f;
				else if(mouseY < y)
					zoom+= 0.2f;
					break;
				case GLUT_MIDDLE_BUTTON:
				break;
		}
		glutPostRedisplay();
	} 
	//printf("x %d, y %d , mouseX %d, mouseY %d\n",x,y,mouseX,mouseY);

	mouseX = x;
	mouseY = y;
}
void ChangeSize(int w, int h)
	{
	GLfloat	 lightPos[] = { -50.f, 50.0f, 100.0f, 1.0f };

	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);
	
	windowWidth = w;
	windowHeight = h;


	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	}


void initialize(){

printf("Kamera Yonlendirme Tuslari:\n\
Sol: <-, Sag: ->, Yukari: ^ ok, Asagi: v ok, Derinlik: + ve -, Zoom: / ve *\n\
Nesne Secimi: F1, F2 ve F3\n\
Nesne Hareketleri:\n\
X Ekseni: a ve d, Y Ekseni: c ve v, Z Ekseni: w ve s\n");

		xPos = (GLfloat*)malloc(sizeof(GLfloat)*count);
		yPos = (GLfloat*)malloc(sizeof(GLfloat)*count);
		zPos = (GLfloat*)malloc(sizeof(GLfloat)*count);
		color = (GLbyte*)malloc(sizeof(GLbyte)*count);

		xPos[0] = -1.5f;
		yPos[0] = 0.0f;
		zPos[0] = 0.0f;
		color[0] = 0;

		xPos[1] = 0.0f;
		yPos[1] = 0.0f;
		zPos[1] = -1.5f;
		color[1] = 1;

		xPos[2] = 1.5f;
		yPos[2] = 0.0f;
		zPos[2] = 0.0f;
		color[2] = 2;

}
int main(int argc, char* argv[])
	{
	initialize();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Ödev 2");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(KeyboardKeys);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();

	return 0;
	}
