/*
 *  Textures and Lighting
 *
 *  Demonstrates using lighting and textures.
 *
 *  Key bindings:
 *  l          Toggle lighting on/off
 *  t          Change textures
 *  m          Toggles texture mode modulate/replace
 *  a/A        decrease/increase ambient light
 *  d/D        decrease/increase diffuse light
 *  s/S        decrease/increase specular light
 *  e/E        decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  []         Lower/rise light
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int mode=0;       //  Texture mode
int displayMode = 0; //  What to display
int ntex=0;       //  Cube faces
int axes=1;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=1;      //  Lighting
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
int fov = 55; // Field of View
double dim=10;   //  Size of world
int    box=1;    //  Draw sky
int    sky[2];   //  Sky textures
int    park[6];  // Park textures
// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[9]; // Texture names
char *text[] = {"Orthogonal","Prespective","FPS"}; 

 double eyeX = 0;
 double eyeY = 0;
 double eyeZ = 0;

 double dx = 1;
 double dy = 1;
 double dz = 1;

 double atX = 0;
 double atY = 0;
 double atZ = 0;

 int angle = 325;
 float fraction = 1.25f;


#define PI 3.1415927
#define DEF_D 5

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 *     wiht texture # tex
 */
static void cube(double x,double y,double z,
  double dx,double dy,double dz,
  double th,
  int tex)
{
//  Set specular color to white
float white[] = {1,1,1,1};
float Emission[]  = {0.0,0.0,0.01*emission,1.0};
glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
//  Save transformation
glPushMatrix();
//  Offset, scale and rotate
glTranslated(x,y,z);
glRotated(th,0,1,0);
glScaled(dx,dy,dz);
//  Enable textures
glEnable(GL_TEXTURE_2D);
glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
glColor3f(1,1,1);
glBindTexture(GL_TEXTURE_2D,texture[tex]);
//  Front
//glColor3f(1,0,0);
//if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
glBegin(GL_QUADS);
glNormal3f( 0, 0, 1);
glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
glEnd();
//  Back
//glColor3f(0,0,1);
//if (ntex) glBindTexture(GL_TEXTURE_2D,texture[2]);
glBegin(GL_QUADS);
glNormal3f( 0, 0,-1);
glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
glEnd();
//  Right
//glColor3f(1,1,0);
//if (ntex) glBindTexture(GL_TEXTURE_2D,texture[3]);
glBegin(GL_QUADS);
glNormal3f(+1, 0, 0);
glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
glEnd();
//  Left
//glColor3f(0,1,0);
//if (ntex) glBindTexture(GL_TEXTURE_2D,texture[4]);
glBegin(GL_QUADS);
glNormal3f(-1, 0, 0);
glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
glEnd();
//  Top
//glColor3f(0,1,1);
//if (ntex) glBindTexture(GL_TEXTURE_2D,texture[5]);
glBegin(GL_QUADS);
glNormal3f( 0,+1, 0);
glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
glEnd();
//  Bottom
//glColor3f(1,0,1);
//if (ntex) glBindTexture(GL_TEXTURE_2D,texture[6]);
glBegin(GL_QUADS);
glNormal3f( 0,-1, 0);
glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
glEnd();
//  Undo transformations and textures
glPopMatrix();
glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a stud
 */
static void stud(float x,float y, float z,
                  double dx,double dy,double dz,
                  double angle,
                  int tex)
{
   int th;
   glPushMatrix();
   glTranslatef(x,y,z);
   glRotated(angle,1,0,0);
   glScaled(dx,dy,dz);
   //  Top
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0,0,1);
   glVertex3f(0,0,1);
   for (th=0;th<=360;th+=15)
      glVertex3f(Cos(th),Sin(th),1);
   glEnd();
   //  Sides
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=15)
   {
      glNormal3f(Cos(th),Sin(th),0);
      glVertex3f(Cos(th),Sin(th),1);
      glVertex3f(Cos(th),Sin(th),0);
   }
   glEnd();
   glPopMatrix();
}



void houseWithRoof(double x,double y,double z,
  double length, double width, double height,
  double th, double roofHeight)
{

//  Set specular color to white
float white[] = {1,1,1,1};
float black[] = {0,0,0,1};
glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);  
//  Save transformation
glPushMatrix();
//  Offset
glTranslated(x,y,z);
glRotated(th,0,1,0);

glScaled(length,width,height);
//  Enable textures
glEnable(GL_TEXTURE_2D);
glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
glColor3f(1,1,1);
glBindTexture(GL_TEXTURE_2D,texture[0]);
//  House
//  Enable Polygon Offset
glEnable(GL_POLYGON_OFFSET_FILL);
glPolygonOffset(1,1);
//if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
glBegin(GL_QUADS);


//  Front
//glColor3f(1,0,0);

//Normal Vector
glNormal3f(1, 0, 0);

glTexCoord2f(0,0);glVertex3f(1, 0, 0);
glTexCoord2f(0,1);glVertex3f(1, 1, 0);
glTexCoord2f(1,1);glVertex3f(1, 1, 1);
glTexCoord2f(1,0);glVertex3f(1, 0, 1);

glEnd();

//  Disable polygon offset
glDisable(GL_POLYGON_OFFSET_FILL);
//glDisable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D,texture[5]);
glBegin(GL_QUADS);
//door
glColor3f(1,1,1);

//Normal Vector
glNormal3f(1, 0, 0);

glTexCoord2f(0,0);glVertex3f(1, 0, 0.1);
glTexCoord2f(0,1);glVertex3f(1, 0.6, 0.1);
glTexCoord2f(1,1);glVertex3f(1, 0.6, 0.3);
glTexCoord2f(1,0);glVertex3f(1, 0, 0.3);
glEnd();

// window
glBindTexture(GL_TEXTURE_2D,texture[6]);
glBegin(GL_QUADS);
//glColor3f( 0.74902 , 0.847059 , 0.847059); // light blue
glColor3f(1,1,1);
//Normal Vector
glNormal3f(1, 0, 0);
glTexCoord2f(0,0);glVertex3f(1, 0.3, 0.5);
glTexCoord2f(0,1);glVertex3f(1, 0.6, 0.5);
glTexCoord2f(1,1);glVertex3f(1, 0.6, 0.7);
glTexCoord2f(1,0);glVertex3f(1, 0.3, 0.7);
glEnd();

glBindTexture(GL_TEXTURE_2D,texture[0]);
glBegin(GL_QUADS);
//if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
//Back
glColor3f(1,1,1);
//Normal Vector
glNormal3f(-1, 0, 0);

glTexCoord2f(0,0);glVertex3f(0, 0, 0);
glTexCoord2f(0,1);glVertex3f(0, 1, 0);
glTexCoord2f(1,1);glVertex3f(0, 1, 1);
glTexCoord2f(1,0);glVertex3f(0, 0, 1);


// left
//glColor3f(1,0,0);
//Normal Vector
glNormal3f(0 , 0, 1);

glTexCoord2f(1,0);glVertex3f(0, 0, 1);
glTexCoord2f(0,0);glVertex3f(1, 0, 1);
glTexCoord2f(0,1);glVertex3f(1, 1, 1);
glTexCoord2f(1,1);glVertex3f(0, 1, 1);




// right
//glColor3f(1,0,0);
//Normal Vector
glNormal3f(0, 0, -1);



glTexCoord2f(1,0);glVertex3f(0, 0, 0);
glTexCoord2f(0,0);glVertex3f(1, 0, 0);
glTexCoord2f(0,1);glVertex3f(1, 1, 0);
glTexCoord2f(1,1);glVertex3f(0, 1, 0);
glEnd();




if (roofHeight == 0)
{


  //top
  glBegin(GL_QUADS);
  glColor3f( 0.647059 , 0.164706 , 0.164706); // brown

  glNormal3f(0, 1, 0);
  glVertex3f(0, 1, 0);
  //glColor3f(1,0,1);
  glVertex3f(0, 1, 1);
  //glColor3f(0,1,1);
  glVertex3f(1, 1, 1);
  //glColor3f(0,1,0);
  glVertex3f(1, 1, 0);
  glEnd();

}

else
{
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glBegin(GL_TRIANGLES);

  glColor3f( 0.647059 , 0.164706 , 0.164706); // brown

  //right triangle in the roof
  glNormal3f(0, 0, -0.5);

  glTexCoord2f(1,0);glVertex3f(0, 1, 0);
  glTexCoord2f(1,1);glVertex3f(0.5, roofHeight, 0);
  glTexCoord2f(0,0);glVertex3f(1, 1 ,0);

  

  // left triangle in the roof
  glNormal3f(0, 0, 0.5);

  glTexCoord2f(1,0);glVertex3f(1, 1, 1);
  glTexCoord2f(1,1);glVertex3f(0.5, roofHeight, 1);
  glTexCoord2f(0,0);glVertex3f(0, 1 ,1);

  glEnd();

  glBegin(GL_QUADS);
  glColor3f( 0.647059 , 0.164706 , 0.164706); // brown
  
  // back roof
  glNormal3f(-0.5, 0.5, 0);

  glTexCoord2f(1,0);glVertex3f(0, 1, 1);
  glTexCoord2f(1,1);glVertex3f(0.5, roofHeight, 1);
  glTexCoord2f(0,1);glVertex3f(0.5, roofHeight, 0);
  glTexCoord2f(0,0);glVertex3f(0, 1, 0);

  glColor3f( 0.647059 , 0.164706 , 0.164706); // brown
  
  // front roof
  glNormal3f(0.5, -0.5, 0);

  glTexCoord2f(1,0);glVertex3f(1, 1, 0);
  glTexCoord2f(1,1);glVertex3f(0.5, roofHeight, 0);
  glTexCoord2f(0,1);glVertex3f(0.5, roofHeight, 1);
  glTexCoord2f(0,0);glVertex3f(1, 1, 1);

  glEnd();

}

  //  Undo transformations
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}


void grass()
{
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Enable textures
glEnable(GL_TEXTURE_2D);
glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
glColor3f(1,1,1);
  //glColor3f(0.560784 , 0.737255 , 0.560784);
  glBindTexture(GL_TEXTURE_2D,texture[2]);
  
  
   //  Enable Polygon Offset

   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   // glTranslated(0, 1.001,0);
   //glRotated(-90,1,0,0);
   glScaled(1.75, 1.75, 1.75);
   glBegin(GL_POLYGON);
  
   glNormal3f(0, 1, 0);
   glTexCoord2f(0,0);glVertex3f(0, 0, 0);
   glTexCoord2f(10,0);glVertex3f(0, 0, 1);
   glTexCoord2f(10,10);glVertex3f(1, 0, 1);
   glTexCoord2f(0,10);glVertex3f(1, 0, 0);

   glNormal3f(0, 1, 0);
   glTexCoord2f(0,0);glVertex3f(0, 0, 0);
   glTexCoord2f(10,0);glVertex3f(0, 0, 1);
   glTexCoord2f(10,10); glVertex3f(-1, 0, 1);
   glTexCoord2f(0,10);glVertex3f(-1, 0, 0);
   
   glNormal3f(0, 1, 0);
   glTexCoord2f(0,0);glVertex3f(0, 0, 0);
   glTexCoord2f(10,0);glVertex3f(1, 0, 0);
   glTexCoord2f(10,10);glVertex3f(1, 0, -1);
   glTexCoord2f(0,10);glVertex3f(0, 0, -1);

   glNormal3f(0, 1, 0);
   glTexCoord2f(0,0);glVertex3f(0, 0, 0);
   glTexCoord2f(10,0);glVertex3f(0, 0, -1);
   glTexCoord2f(10,10);glVertex3f(-1, 0, -1);
   glTexCoord2f(0,10);glVertex3f(-1, 0, 0);

   glEnd();
   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

void draw_cylinder(GLfloat radius, GLfloat height,
  GLubyte R, GLubyte G,GLubyte B,
  double x1,double y1,double z1,
  double xt, double yt, double zt)
{
  GLfloat x              = 0.0;
  GLfloat y              = 0.0;
  GLfloat angle          = 0.0;
  GLfloat angle_stepsize = 0.1;

  //  Save transformation
  glPushMatrix();
  glTranslatef(x1,0,z1);
  glRotated(-90,1,0,0);
  glScaled(xt, yt, zt);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
  glColor3f(1,1,1);
    //glColor3f(0.560784 , 0.737255 , 0.560784);
  glBindTexture(GL_TEXTURE_2D,texture[3]);

  /** Draw the tube */
  glColor3ub(R-40,G-40,B-40);
  glBegin(GL_QUAD_STRIP);
  angle = 0.0;
  while( angle < 2*PI ) 
  {
      x = radius * cos(angle);
      y = radius * sin(angle);
      float tc = (angle / (float)(2 * PI));
      glNormal3f(cos(angle) , sin(angle), 0);
      glTexCoord2f(tc, 0.0);
      glVertex3f(x, y , height);
      glTexCoord2f(tc, 1.0);
      glVertex3f(x, y , 0.0);
      angle = angle + angle_stepsize;
  }
  glVertex3f(radius, 0.0, height);
  glVertex3f(radius, 0.0, 0.0);
  glEnd();

  /** Draw the circle on top of cylinder */
  glColor3ub(R,G,B);
  glBegin(GL_POLYGON);
  angle = 0.0;
  while( angle < 2*PI ) 
  {
      x = radius * cos(angle);
      y = radius * sin(angle);
      glVertex3f(x, y , height);
      angle = angle + angle_stepsize;
  }
  glVertex3f(radius, 0.0, height);
  glEnd();

  //  Undo transformations
 glPopMatrix();
 glDisable(GL_TEXTURE_2D);
}


/*
*  Draw solid tree
*    at (x,y)
*    scale (xt,yt,zt)
*    
*/
void basicTree(double x,double z,
  double xt, double yt, double zt)
{
  double y = 0.5;
  draw_cylinder(0.07, 1.0, 255, 160, 100,x, y, z, xt,yt,zt);
   //  Save transformation
  glPushMatrix();
 // glColor3f(  0.137255 , 0.556863 , 0.137255); // ForestGreen
  glTranslatef(x,y,z);
  glRotated(-90,1,0,0);
  glScaled(xt, yt, zt);

    //  Enable textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
  glColor3f(1,1,1);
  glBindTexture(GL_TEXTURE_2D,texture[4]);
   
  float radius = 0.25;
  float height = 0.25;
     // draw the upper part of the cone
  for(int i = 0 ; i < 4 ; i++)
  {   
    glBegin(GL_TRIANGLES);
    
    for (int angle = 0; angle < 360; angle+=DEF_D) 
    {
      glNormal3f(0,0,1);
      glVertex3f(0, 0, height);
      glNormal3f(sin(angle) * radius, cos(angle) * radius, -radius);
      glTexCoord2f(sin(angle) * radius *1000, cos(angle) * radius *1000);
      glVertex3f(sin(angle) * radius, cos(angle) * radius, 0);
      //glVertex3f(sin(angle + DEF_D) * radius, cos(angle + DEF_D) * radius, 0);

    }
    
    glEnd();
    glTranslatef(0,0,0.15);
}

  
 //  Undo transformations
 glPopMatrix();
 glDisable(GL_TEXTURE_2D);
}






/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

/* 
 *  Draw sky box
 */
static void Sky(double D)
{

  printf("D = %f\n", D);
  
  double offset = 2.0;
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
   //glTranslatef(0,5,0);
   //  Sides
   glBindTexture(GL_TEXTURE_2D,park[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);
   glEnd();



   glBindTexture(GL_TEXTURE_2D,park[4]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(1,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0,1); glVertex3f(+D,+D,-D);
   glEnd();
   
   glBindTexture(GL_TEXTURE_2D,park[3]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(1,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(+D,+D,+D);
   glEnd();
   
   glBindTexture(GL_TEXTURE_2D,park[1]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,park[2]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(1,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(1,0); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,0); glVertex3f(-D,+D,-D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,park[5]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(1,1); glVertex3f(+D,-D,-D);
   glTexCoord2f(0,1); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Length of axes
   const double len=10.0;
   //  Eye position
   //double Ex = -2*dim*Sin(th)*Cos(ph);
   //double Ey = +2*dim        *Sin(ph);
   //double Ez = +2*dim*Cos(th)*Cos(ph);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Set perspective
   glLoadIdentity();
   switch(displayMode)
   {
       case 0:
           //  Orthogonal
           glRotatef(ph,1,0,0);
           glRotatef(th,0,1,0);
           break;
       case 1:
           // Prespective
           eyeX = -2 * dim * Sin(th) * Cos(ph);
           eyeY = 2 * dim * Sin(ph);
           eyeZ = 2 * dim * Cos(th) * Cos(ph);
         
        // gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ); 
           gluLookAt(eyeX, eyeY, eyeZ, 0, 0, 0, 0, Cos(ph),0); 
           break;

       case 2:
                 
           // FPS navigation
           dx = Sin(angle);
           dz = Cos(angle);
           printf("%f ,%f\n ", dx,dz);
           gluLookAt(eyeX, 1, eyeZ,   eyeX + dx , 1, eyeZ + dz,      0, 1 ,0); 
           //gluLookAt(eyeX, 1, eyeZ, eyeX + ex, 1, eyeZ + ez, 0, 1, 0);
           //gluLookAt(eyeX, 1, eyeY, eyeX + ex, eyeY + ey,1, 0,0,1);

          printf("eyeX = %f eyeZ = %f lx = %f lz = %f angle= %d\n",eyeX , eyeZ, dx, dz,  angle);
           break;

   }
   //gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);

  //  Draw sky
  if (box) Sky(10*dim);
  //  Light switch
  if (light)
  {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);
   //  Draw scene
   //cube(0,0,0 , 0.5,0.5,0.5 , 0);
      //  Draw scene
      cube(0,-10*dim,0 ,3,0.25,4, 0 ,3);
      //grass();
      //basicTree(-1, 1,  0.5,0.5,0.5);
      //basicTree(1, -1,  0.5,0.5,0.5);
      //basicTree(-0.5, -1.5,  0.5,0.5,0.5);
      //houseWithRoof(0,0,0, 7,8,10 , 0, 1.5);
    
      //houseWithRoof(-1,0,0, 0.5,0.5,0.8 , 90, 1.5);
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Light=%s Texture=%s  Mode = %s",th,ph,dim,light?"On":"Off",mode?"Replace":"Modulate",text[displayMode]);
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void reproject(int mode)
{
  if (mode)
  {
      Project(fov, asp, dim);
  }
  else
  {
      Project(0, asp, dim);
  }
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
   {
     if (displayMode == 2)
     {
      eyeX += dx * fraction;
      eyeZ +=  dz * fraction;  
     }
     else
     {
      ph += 5;
     }
   }
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   reproject(displayMode);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}



/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
  if (ch == 27)
      exit(0);
   //  Reset view angle
  else if (ch == '0')
    th = ph = 0;
  else if (ch == 'w' || ch == 'W')
  {
      eyeX += dx * fraction;
      eyeZ +=  dz * fraction;   
  }
  else if (ch == 's' || ch == 'S')
  {
      eyeX -= dx * fraction;
      eyeZ -=  dz * fraction;
  }
  else if (ch == 'a' || ch == 'A')
  {
      angle += 5;
  } 
  else if (ch == 'd' || ch == 'D')
  {
      angle -= 5;
  }
  //  Switch display mode
  else if (ch == 'm' || ch == 'M')
  {
    displayMode++;
    if(displayMode > 2)
    {
      displayMode = 0 ;
      dim = 20;
    }
    else if(displayMode == 1)
    {
        //th = ph = 0;
    }
    else if(displayMode == 2)
    {
      eyeX = 2 *10;
      eyeZ = -3 *10;
    }
  }
  //  Toggle axes
  else if (ch == 'x' || ch == 'X')
    axes = 1-axes;
  //  Toggle lighting
  else if (ch == 'l' || ch == 'L')
    light = 1-light;
  //  Toggle textures mode
  else if (ch == 't')
    ntex = 1-ntex;
  //  Light elevation
  else if (ch=='[')
    ylight -= 0.1;
  else if (ch==']')
    ylight += 0.1;
  //  Ambient level
  else if (ch=='a' && ambient>0)
    ambient -= 5;
  else if (ch=='A' && ambient<100)
    ambient += 5;
  //  Diffuse level
  else if (ch=='d' && diffuse>0)
    diffuse -= 5;
  else if (ch=='D' && diffuse<100)
    diffuse += 5;
  //  Specular level
  else if (ch=='s' && specular>0)
    specular -= 5;
  else if (ch=='S' && specular<100)
    specular += 5;
  //  Emission level
  else if (ch=='e' && emission>0)
    emission -= 5;
  else if (ch=='E' && emission<100)
    emission += 5;
  //  Shininess level
  else if (ch=='n' && shininess>-1)
    shininess -= 1;
  else if (ch=='N' && shininess<7)
    shininess += 1;
  //  Repitition
  else if (ch=='+')
    rep++;
  else if (ch=='-' && rep>1)
    rep--;
  //  Translate shininess power to value (-1 => 0)
  shiny = shininess<0 ? 0 : pow(2.0,shininess);
  //  Reproject
  reproject(displayMode);
   //  Tell GLUT it is necessary to redisplay the scene
  glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   reproject(displayMode);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT
  glutInit(&argc,argv);
  //  Request double buffered, true color window with Z buffering at 600x600
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(1000,1000);
  glutCreateWindow("The 'Village' by Mahmoud Aljarrash with Textures and Lighting");
  //  Set callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutKeyboardFunc(key);
  glutIdleFunc(idle);
  //  Load textures
  texture[0] = LoadTexBMP("Masonry_Stone.bmp");
  texture[1] = LoadTexBMP("ShingleRoof_18dpi.bmp");
  texture[2] = LoadTexBMP("Grass.bmp");
  texture[3] = LoadTexBMP("BarkDecidious.bmp");
  texture[4] = LoadTexBMP("ivy.bmp");
  texture[5] = LoadTexBMP("door.bmp");
  texture[6] = LoadTexBMP("WindowsHouseOld.bmp");
  texture[7] = LoadTexBMP("Leaves0017.bmp");
  texture[8] = LoadTexBMP("Leaves0206.bmp");

  //  Load skybox texture
  sky[0] = LoadTexBMP("sky0.bmp");
  sky[1] = LoadTexBMP("sky1.bmp");

  //  Load park texture
  park[0] = LoadTexBMP("posx.bmp");
  park[1] = LoadTexBMP("posz.bmp");
  park[2] = LoadTexBMP("posy.bmp");
  park[3] = LoadTexBMP("negx.bmp");
  park[4] = LoadTexBMP("negz.bmp");
  park[5] = LoadTexBMP("negy.bmp");



   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
