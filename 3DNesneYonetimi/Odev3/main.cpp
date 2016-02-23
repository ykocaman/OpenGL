#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glut.h"
#include <stdio.h>
#include <math.h>

// Rotation amounts
static GLfloat xRot = 270.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;

GLfloat *xPos;
GLfloat *yPos;
GLfloat *zPos;

GLbyte active=0;
GLboolean locked=false;
static GLbyte count=8;

GLfloat zoom=8.0f;
GLint windowWidth;
GLint windowHeight;

GLint mouseButton;
GLint mouseState;
GLint mouseX;
GLint mouseY;

GLint i;

GLfloat Pi = 3.14f;

// Light values and coordinates
GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };

void drawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC)
{
	float tStep = (Pi) / (float)uiSlices;
	float sStep = (Pi) / (float)uiStacks;
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	for(float t = -Pi/2; t <= (Pi/2)+.0001; t += tStep)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(float s = -Pi; s <= Pi+.0001; s += sStep)
		{
			glVertex3f(fA * cos(t) * cos(s), fB * cos(t) * sin(s), fC * sin(t));
			glVertex3f(fA * cos(t+tStep) * cos(s), fB * cos(t+tStep) * sin(s), fC * sin(t+tStep));
		}
		glEnd();
	}
}
    


// Called to draw scene
void RenderScene(void)
	{
	// Clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	//--- BEGIN ZOOM ----
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (windowWidth <= windowHeight) 
		glOrtho (-zoom, zoom, -zoom*windowHeight/windowWidth, zoom*windowHeight/windowWidth, -zoom, zoom);
    else 
		glOrtho (-zoom*windowWidth/windowHeight, zoom*windowWidth/windowHeight, -zoom, zoom, -zoom, zoom);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//--- END ZOOM ----

	if(locked){
		xPos[2] = xPos[1];  
		yPos[2] = yPos[1] - 0.1f;  
		zPos[2] = zPos[1];  
		xPos[4] = xPos[3];  
		yPos[4] = yPos[3] - 0.1f;  
		zPos[4] = zPos[3];  
	}

	for(i=0;i<count;i++){
		glPushMatrix();
		glTranslatef(0.0f,0.0f,0.0f);
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		glRotatef(zRot, 0.0f, 0.0f, 1.0f);
		glTranslatef(xPos[i],yPos[i],zPos[i]);

		switch(i){
		case 0:
		case 5:
		case 6:
			glColor3ub(255,255,255);
		break;
		case 1:
			glColor3ub(150,150,255);
		break;
		case 2:
			glColor3ub(0,0,255);
		break;
		case 3:
			glColor3ub(150,255,150);
		break;
		case 4:
			glColor3ub(0,255,0);
		break;
		case 7:
			glColor3ub(255,105,147);
		default:
			glColor3ub(255,0,0);
		}

		switch(i){
		case 0:
			glutSolidSphere(3.0, 50, 50);
		break;
		case 1:
		case 3:
			  drawEllipsoid(10.0f, 10.0f,0.8f,0.3f,0.3f);
		break;
		case 2:
		case 4:
			glutSolidSphere(0.30, 50, 50);
		break;
		case 5:
		case 6:
			glutSolidCone(0.5f, 1.6f, 50, 50);
		break;
		case 7:
			glutSolidCone(0.5f, 0.8f, 50, 50);
		break;
		}
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
		zRot -= 5.0f;
	break;
	case GLUT_KEY_RIGHT:
		zRot += 5.0f;
	break;
	case GLUT_KEY_F1:
		active=0;
	break;
	case GLUT_KEY_F2:
		active=1;
	break;
	case GLUT_KEY_F3:
		active=2;
		if(locked)
			active--;
	break;
	case GLUT_KEY_F4:
		active=3;
	break;
	case GLUT_KEY_F5:
		active=4;
		if(locked)
			active--;
	break;
	case GLUT_KEY_F6:
		active=5;
	break;
	case GLUT_KEY_F7:
		active=6;
	break;
	case GLUT_KEY_F8:
		active=7;
	break;
	case GLUT_KEY_F9:
		active=8;
	break;
	}

	glutPostRedisplay();
	}


void KeyboardKeys(unsigned char key, int x, int y)
	{
	switch (key){
	case 'w':
	case 'W':
		zPos[active] += 0.1f;
	break;
	case 's':
	case 'S':
		zPos[active] -= 0.1f;;
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
		yPos[active] -= 0.1f;
	break;
	case 'v':
	case 'V':
		yPos[active] += 0.1f;
	break;
	case '+':
		yRot+= 5.0f;
	break;
	case '-':
		yRot-= 5.0f;
	break;
	case '*':
		if(zoom > 4.0f)
			zoom-= 0.5f;
	break;
	case '/':
		zoom+=  0.5f;
	break;
	case 'k':
	case 'K':
		if(locked)
			locked = false;
		else
			locked = true;
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
					zRot -= (mouseX - x);
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
Nesne Secimi: F1, F2, F3 ... F8\n\
Kilitleme: k \n\
Nesne Hareketleri:\n\
X Ekseni: a ve d, Y Ekseni: c ve v, Z Ekseni: w ve s\n");

		xPos = (GLfloat*)malloc(sizeof(GLfloat)*count);
		yPos = (GLfloat*)malloc(sizeof(GLfloat)*count);
		zPos = (GLfloat*)malloc(sizeof(GLfloat)*count);

		xPos[i] = 0.0f;
		yPos[i] = 0.0f;
		zPos[i] = 0.0f;	
		i++;
		xPos[i] = -4.0f;
		yPos[i] = 0.0f;
		zPos[i] = 5.0f;	

		for(i=2;i<count;i++){
			xPos[i] = xPos[i-1] + 1.5f;
			yPos[i] = yPos[i-1];
			zPos[i] = zPos[i-1];
		}

}
int main(int argc, char* argv[])
	{
	initialize();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Ödev 3");
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
