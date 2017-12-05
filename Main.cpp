#define _CRT_SECURE_NO_DEPRECATE

#include <GL/glew.h>
#include<GLFW\glfw3.h>

#include<windows.h>
#include<GL/freeglut.h>
#include<GL/GLAux.h>
#   pragma comment (lib, "glaux.lib")    /* link with Win32 GLUT lib */

#include<iostream>
#include"ObjModel.h"
using namespace std;
ObjModel model;
unsigned int id;

// angle of rotation for the camera direction
float angle = 0.0f;
float xpos = 0.0 , ypos = -30.0 , zpos = -80;

// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;

// XZ position of the camera
float x = 0.0f, z = 5.0f;

float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;




void CreateTexture(UINT &texture, LPSTR strFileName)
{
	AUX_RGBImageRec *pBitmap = NULL;
	if (!strFileName)									// Return from the function if no file name was passed in
		return;
	pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data
	if (pBitmap == NULL)									// If we can't load the file, quit!
		exit(0);

	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}

		free(pBitmap);									// Free the bitmap structure
	}
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_UP: 
		deltaMove =-5.0f;
		
		break;
	case GLUT_KEY_DOWN:
		deltaMove = 5.0f;
		break;
	case GLUT_KEY_LEFT :
		xpos -= 1;
		if (xpos < -50&& deltaMove==0)
			xpos = 50.0;
		break;
	case GLUT_KEY_RIGHT:
		xpos += 1;
		if (xpos > 50 && deltaMove == 0)
			xpos = -50.0;
		break;
	
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}


int p = 0;
void charKey(unsigned char k, int x, int y)
{
	glutPostRedisplay();
	switch (k)
	{
	case 27:
		exit(0);
		break;
    
	case 'w' :
	case 'W':
		ypos += 1;
		break;
	case 's':
	case 'S' :
		ypos -= 1;
		break;
	case 'p':
	case 'P':
		p = 1;
		break;

	}
}

float ang = 90.0 , angle1=0;
float xang = 100 , yang =0.0 , zang = 0.0;

void display(void)
{

	if (deltaMove)
		computePos(deltaMove);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(x, 1.0f, z,
		x + lx, 0.8f, z + lz,
		0.0f, 1.0f, 0.0f);
	glTranslatef(xpos, ypos,zpos);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(ang,xang ,yang, zang);
	if (p == 0) {
		angle1 += 0.3;
		
		glRotatef(angle1, 0, 1, 1);
		
	}


	if (model.hasTextureCoord())
	{
		glBindTexture(GL_TEXTURE_2D, id);
		glEnable(GL_TEXTURE_2D);
	}
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0;i<model.vectorOfFace.size();i++)
		{

			if (model.hasNormalCoord())
				glNormal3d(model.vectorOfNormal[model.vectorOfFace[i].firstVertex[2]].xn,
					model.vectorOfNormal[model.vectorOfFace[i].firstVertex[2]].yn,
					model.vectorOfNormal[model.vectorOfFace[i].firstVertex[2]].zn);
			if (model.hasTextureCoord())
				glTexCoord2f(model.vectorOfTexture[model.vectorOfFace[i].firstVertex[1]].s,
					model.vectorOfTexture[model.vectorOfFace[i].firstVertex[1]].t);
			glVertex3f(model.vectorOfVertex[model.vectorOfFace[i].firstVertex[0]].x,
				model.vectorOfVertex[model.vectorOfFace[i].firstVertex[0]].y,
				model.vectorOfVertex[model.vectorOfFace[i].firstVertex[0]].z);
			if (model.hasNormalCoord())
				glNormal3d(model.vectorOfNormal[model.vectorOfFace[i].secondVertex[2]].xn,
					model.vectorOfNormal[model.vectorOfFace[i].secondVertex[2]].yn,
					model.vectorOfNormal[model.vectorOfFace[i].secondVertex[2]].zn);
			if (model.hasTextureCoord())
				glTexCoord2f(model.vectorOfTexture[model.vectorOfFace[i].secondVertex[1]].s,
					model.vectorOfTexture[model.vectorOfFace[i].secondVertex[1]].t);
			glVertex3f(model.vectorOfVertex[model.vectorOfFace[i].secondVertex[0]].x,
				model.vectorOfVertex[model.vectorOfFace[i].secondVertex[0]].y,
				model.vectorOfVertex[model.vectorOfFace[i].secondVertex[0]].z);

			if (model.hasNormalCoord())
				glNormal3d(model.vectorOfNormal[model.vectorOfFace[i].thirdVertex[2]].xn,
					model.vectorOfNormal[model.vectorOfFace[i].thirdVertex[2]].yn,
					model.vectorOfNormal[model.vectorOfFace[i].thirdVertex[2]].zn);

			if (model.hasTextureCoord())
				glTexCoord2f(model.vectorOfTexture[model.vectorOfFace[i].thirdVertex[1]].s,
					model.vectorOfTexture[model.vectorOfFace[i].thirdVertex[1]].t);
			glVertex3f(model.vectorOfVertex[model.vectorOfFace[i].thirdVertex[0]].x,
				model.vectorOfVertex[model.vectorOfFace[i].thirdVertex[0]].y,
				model.vectorOfVertex[model.vectorOfFace[i].thirdVertex[0]].z);
		}
	}
	glEnd();
	


	glutPostRedisplay();
	glutSwapBuffers();
	
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);

	//gluPerspective(65, 1, .1, -100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 50);

	model.load("spaceship.obj");
	glutCreateWindow("wavefront file loader ");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1, 1, 1);
	glClearColor(0, 0, 0, 1);
	CreateTexture(id, "spaceshiptexture.bmp");
	

	glutDisplayFunc(display);

	glutReshapeFunc(reshape);
	glutIgnoreKeyRepeat(0);
	glutKeyboardFunc(charKey);

	//glutIgnoreKeyRepeat(1);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutMainLoop();
	return 0;
}
