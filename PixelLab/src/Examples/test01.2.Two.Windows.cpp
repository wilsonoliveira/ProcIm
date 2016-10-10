/*
  Name:        test01.2.Two.Windows.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Author:      Bruno José Dembogurski
  Date:        27/01/2011
  Last Update: 27/01/2016
  Description: Managing two windows using PixelLab.
*/

#include  "pixelLab.h"
#ifdef WIN32
#include <windows.h>
#endif
#include "GL/glut.h"
#include <stdio.h>
#include <stdlib.h>

// Image Objects
PixelLab *img = NULL, *imgOriginal = NULL;
int window1 = 0;
int window2 = 0;
int brightnessSlider = 128;

void idle()
{
   glutPostRedisplay();
}

static void display1(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   // reverse 'Y' to allow correct mouse click (0 value is in the upper left corner for mouse functions)
   gluOrtho2D(0, 64, 256, 0); // You could use "glOrtho(0, 64, 256, 0, -5, 5);" instead if you want 3D features

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glBegin(GL_QUADS);
      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex2d( 0, 0);
      glVertex2d(64, 0);
      glColor3f(1.0f, 1.0f, 1.0f);
      glVertex2d(64, 256);
      glVertex2d( 0, 256);
   glEnd();

   glBegin(GL_LINES);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex2d( 0, brightnessSlider);
      glVertex2d(64, brightnessSlider);
   glEnd();

   glutSwapBuffers ();
}

static void display2(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   img->ViewImage();
   glutSwapBuffers();
}

void modifyImage()
{
   int value = brightnessSlider-128; // To make the middle of the slider, zero
   int pixelVal;

   for(int y = 0; y < imgOriginal->GetHeight(); y++)
      for(int x = 0; x < imgOriginal->GetWidth(); x++)
      {
         pixelVal = imgOriginal->GetGrayValue(x, y);
         pixelVal += value;
         if(pixelVal >= 255) pixelVal = 255;
         if(pixelVal < 0)    pixelVal = 0;
         img->SetGrayValue(x, y, (uByte) pixelVal);
      }
   // Update both windows
   glutPostWindowRedisplay	(window1);
   glutPostWindowRedisplay	(window2);
}

// Keyboard
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
      case 27 :
      case 'q':
         exit(0);
      break;
    }
    glutPostRedisplay();
}

// Special Keys callback
void specialKeys(int key, int x, int y)
{
   switch(key)
   {
      case GLUT_KEY_UP:
         brightnessSlider = (brightnessSlider<0) ? 0 : brightnessSlider-1;
         modifyImage();
      break;
      case GLUT_KEY_DOWN:
         brightnessSlider = (brightnessSlider>255) ? 255 : brightnessSlider+1;
         modifyImage();
      break;
   }
   glutPostRedisplay();
}

// Mouse callback - Capture mouse click in the brightness window
void mouse(int button, int state, int x, int y)
{
   if ( button == GLUT_LEFT_BUTTON)
      if(state == GLUT_DOWN)
         brightnessSlider = y;
   modifyImage();
}

// Motion callback - Capture mouse motion when left button is clicked
void motion(int x, int y )
{
   brightnessSlider = y;
   modifyImage();
}

void init()
{
   imgOriginal = new PixelLab();
   imgOriginal->Read("../figs/lenaGray.png");

   img = new PixelLab();
   img->Copy(imgOriginal);

   printf("Change brightness clicking on the left window\n or using the 'up' and 'down' keyboard keys.\n");
}

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   // Init image objects
   init();

   // Create two windows
   window2 = glutCreateWindow("Display Window");
   window1 = glutCreateWindow("Control Window");

   ///////////////////////////////////////////////////////////
   // Control Window
   glutSetWindow (window1);   // Change current window to 1 (all callbacks will be set to this window)
   glutDisplayFunc(display1);
   glutPositionWindow(20, 30);
   glutReshapeWindow(64, 256);
   glutMouseFunc( mouse );
   glutMotionFunc( motion );
   glutSpecialFunc(specialKeys);
   glutKeyboardFunc(key);

   ///////////////////////////////////////////////////////////
   // Display Window
   glutSetWindow (window2); // Change current window to 2
   glutDisplayFunc(display2);
   glutReshapeWindow(imgOriginal->GetWidth(),imgOriginal->GetHeight());
   glutPositionWindow(100, 30);

   glutMainLoop();

   return 1;
}
