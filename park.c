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
double dim=15;   //  Size of world
int    box=0;    //  Draw sky
int    sky[2];   //  Sky textures
int    park[6];  // Park textures
int fireworks=0; // Fireworks
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
int obj;          //  Object display list

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
#define MAX_POINTS 5000

int numPoints;
GLfloat curx, cury,curz;
GLfloat x[MAX_POINTS], y[MAX_POINTS], z[MAX_POINTS];
GLfloat xacc[MAX_POINTS], yacc[MAX_POINTS], zacc[MAX_POINTS];
GLfloat red, green, blue;
int step; int length;

void fireworks_initialize()
{ 
    int j; double temp, temp2,temp3;

  numPoints = drand48()*(MAX_POINTS-1);
  curx = -0.5 + drand48();
  cury = 10.0 + drand48();
  curz = +0.25 + drand48();

  red = 0.5 + 0.5*drand48();
  green = 0.5 + 0.5*drand48();
  blue = 0.5 + 0.5*drand48();
  glPointSize(1.5); 
  step = 0;
  length = 700 + 300*drand48();


  /* initialize the blast */
  for (j=0 ; j<numPoints ; j++ ) {
  x[j] = curx;
  y[j] = cury;
  z[j] = curz;
  temp = drand48()*5;
  temp2 = drand48()*2.0*M_PI;
  temp3 = drand48()*M_PI;
  xacc[j] = (cos(temp2) * sin(temp3) * temp)/length;
  yacc[j] = (sin(temp2) * sin(temp3) * temp)/length;
  zacc[j] = (cos(temp3) * temp)/length;


}

}

void draw_blast(void)
{ 
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
   // glTranslated(drand48(),drand48(),drand48());
    int i;
double glow = (length - step) / (double)length;
glColor3f(red*glow, green*glow, blue*glow);
glBegin(GL_POINTS);
for (i=0;i<numPoints;i++) {
x[i] += xacc[i];
y[i] += yacc[i];
z[i] += zacc[i];
glVertex3f(x[i], y[i], z[i]);
}
glEnd();
glFlush();
glPopMatrix();
//glutSwapBuffers();
}
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
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 *     wiht texture # tex
 */
static void cube2(double x,double y,double z,
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

    glBegin(GL_QUADS);
    glNormal3f( 0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(+1, +0, +1);
    glTexCoord2f(1,0); glVertex3f(+1, +0, +0);
    glTexCoord2f(1,1); glVertex3f(+1, +1, +0);
    glTexCoord2f(0,1); glVertex3f(+1, +1, +1);
    glEnd();
    //  Back
    
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0); glVertex3f(+0, +0, +1);
    glTexCoord2f(1,0); glVertex3f(+0, +0, +0);
    glTexCoord2f(1,1); glVertex3f(+0, +1, +0);
    glTexCoord2f(0,1); glVertex3f(+0, +1, +1);
    glEnd();
    //  Right
    
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+0, +0, +0);
    glTexCoord2f(1,0); glVertex3f(+1, +0, +0);
    glTexCoord2f(1,1); glVertex3f(+1, +1, +0);
    glTexCoord2f(0,1); glVertex3f(+0, +1, +0);
    glEnd();
    //  Left

    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+0 ,+0, +1);
    glTexCoord2f(1,0); glVertex3f(+1 ,+0, +1);
    glTexCoord2f(1,1); glVertex3f(+1 ,+1, +1);
    glTexCoord2f(0,1); glVertex3f(+0 ,+1, +1);
    glEnd();
    //  Top

    glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0); glVertex3f(+0 ,+1, +1);
    glTexCoord2f(1,0); glVertex3f(+1, +1, +1);
    glTexCoord2f(1,1); glVertex3f(+1 ,+1, +0);
    glTexCoord2f(0,1); glVertex3f(+0 ,+1, +0);
    glEnd();
    //  Bottom
    
    glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0); glVertex3f(+0, +0, +1);
    glTexCoord2f(1,0); glVertex3f(+1, +0, +1);
    glTexCoord2f(1,1); glVertex3f(+1, +0, +0);
    glTexCoord2f(0,1); glVertex3f(+0, +0, +0);
    glEnd();
    //  Undo transformations and textures
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

static void cube3(double x,double y,double z,
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
    glRotated(th,0,0,1);
    glScaled(dx,dy,dz);
    //  Enable textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,texture[tex]);
    //  Front

    glBegin(GL_QUADS);
    glNormal3f( 0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(+1, +0, +1);
    glTexCoord2f(1,0); glVertex3f(+1, +0, +0);
    glTexCoord2f(1,1); glVertex3f(+1, +1, +0);
    glTexCoord2f(0,1); glVertex3f(+1, +1, +1);
    glEnd();
    //  Back
    
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0); glVertex3f(+0, +0, +1);
    glTexCoord2f(1,0); glVertex3f(+0, +0, +0);
    glTexCoord2f(1,1); glVertex3f(+0, +1, +0);
    glTexCoord2f(0,1); glVertex3f(+0, +1, +1);
    glEnd();
    //  Right
    
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+0, +0, +0);
    glTexCoord2f(1,0); glVertex3f(+1, +0, +0);
    glTexCoord2f(1,1); glVertex3f(+1, +1, +0);
    glTexCoord2f(0,1); glVertex3f(+0, +1, +0);
    glEnd();
    //  Left

    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+0 ,+0, +1);
    glTexCoord2f(1,0); glVertex3f(+1 ,+0, +1);
    glTexCoord2f(1,1); glVertex3f(+1 ,+1, +1);
    glTexCoord2f(0,1); glVertex3f(+0 ,+1, +1);
    glEnd();
    //  Top

    glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0); glVertex3f(+0 ,+1, +1);
    glTexCoord2f(1,0); glVertex3f(+1, +1, +1);
    glTexCoord2f(1,1); glVertex3f(+1 ,+1, +0);
    glTexCoord2f(0,1); glVertex3f(+0 ,+1, +0);
    glEnd();
    //  Bottom
    
    glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0); glVertex3f(+0, +0, +1);
    glTexCoord2f(1,0); glVertex3f(+1, +0, +1);
    glTexCoord2f(1,1); glVertex3f(+1, +0, +0);
    glTexCoord2f(0,1); glVertex3f(+0, +0, +0);
    glEnd();
    //  Undo transformations and textures
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a table side
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 *     wiht texture # tex
 */
static void tableSide(double x,double y,double z,
  double dx,double dy,double dz,
  double th, double offset,
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

    glBegin(GL_QUADS);
    glNormal3f( 0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(+1, +0, +1);
    glTexCoord2f(1,0); glVertex3f(+1, +0, +0);
    glTexCoord2f(1,1); glVertex3f(+1, +1, +0 + offset);
    glTexCoord2f(0,1); glVertex3f(+1, +1, +1 - offset);
    glEnd();
    //  Back
    
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0); glVertex3f(+0, +0, +1);
    glTexCoord2f(1,0); glVertex3f(+0, +0, +0);
    glTexCoord2f(1,1); glVertex3f(+0, +1, +0 + offset);
    glTexCoord2f(0,1); glVertex3f(+0, +1, +1 - offset);
    glEnd();
    //  Right
    
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+0, +0, +0);
    glTexCoord2f(1,0); glVertex3f(+1, +0, +0);
    glTexCoord2f(1,1); glVertex3f(+1, +1, +0 + offset);
    glTexCoord2f(0,1); glVertex3f(+0, +1, +0 + offset);
    glEnd();
    //  Left

    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+0 ,+0, +1);
    glTexCoord2f(1,0); glVertex3f(+1 ,+0, +1);
    glTexCoord2f(1,1); glVertex3f(+1 ,+1, +1 - offset);
    glTexCoord2f(0,1); glVertex3f(+0 ,+1, +1 - offset);
    glEnd();
    //  Top

    glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0); glVertex3f(+0 ,+1, +1 - offset);
    glTexCoord2f(1,0); glVertex3f(+1, +1, +1 - offset);
    glTexCoord2f(1,1); glVertex3f(+1 ,+1, +0 + offset);
    glTexCoord2f(0,1); glVertex3f(+0 ,+1, +0 + offset);
    glEnd();
    //  Bottom
    
    glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0); glVertex3f(+0, +0, +1);
    glTexCoord2f(1,0); glVertex3f(+1, +0, +1);
    glTexCoord2f(1,1); glVertex3f(+1, +0, +0);
    glTexCoord2f(0,1); glVertex3f(+0, +0, +0);
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
                  double angle, int axes,
                  int tex)
{
   int th;
   glPushMatrix();
   glTranslatef(x,y,z);
   if(axes == 0) // rotate on the X-axes
   {
      glRotated(angle,1,0,0);
   }
   else if(axes == 1) // rotate on the Y-axes
   {
      glRotated(angle,0,1,0); 
   }
   else // rotate on the Z-axes
   {
      glRotated(angle,0,0,1);
   }
    
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

void myPoint(x, y, z)
{
  //  Save transformation
  glPushMatrix();
  //glTranslated(x,y,z);
  //  Draw 10 pixel yellow points
  glColor3f(0.255,0.2,0.5);
  glPointSize(5);
  glBegin(GL_POINTS);

  glVertex3d(x, y, z);
  glEnd();
  //  Undo transformations
  glPopMatrix();
}

/*
* Draw Picnic Table
* at (x, y ,Y)
*
*/
void picnicTable(double x,double y,double z,double h)
{
  tableSide(x, y, z, 0.5, h, 4, 0, 0.15, 0);
  tableSide(x + 10, y, z, 0.5, h, 4, 0, 0.15, 0);

  // Top surface
  cube2(x - 0.5 ,h ,z + 0.1, 11.5, 0.2, 0.8, 0 ,1);
  cube2(x - 0.5 ,h ,z + 1.1, 11.5, 0.2, 0.8, 0 ,1);
  cube2(x - 0.5 ,h ,z + 2.1, 11.5, 0.2, 0.8, 0 ,1);
  cube2(x - 0.5 ,h ,z + 3.1, 11.5, 0.2, 0.8, 0 ,1);

  // Seats support bars
  cube2(x + 0.5, y + 0.2 ,z - 2.5, 0.1, h/2, 8.9, 0, 1);
  cube2(x + 9.99, y + 0.2 ,z - 2.5, 0.1, h/2, 8.9, 0, 1);

  // Front seats
  cube2(x - 0.5 ,h/2 + 0.1 ,z + -1.5, 11.5, 0.2, 0.8, 0 ,1);
  cube2(x - 0.5 ,h/2 + 0.1 ,z + -2.5, 11.5, 0.2, 0.8, 0 ,1);


  // Back seats
  cube2(x - 0.5 ,h/2 + 0.1 ,z +4.5, 11.5, 0.2, 0.8, 0 ,1);
  cube2(x - 0.5 ,h/2 + 0.1 ,z +5.5, 11.5, 0.2, 0.8, 0 ,1);
}

/*
* Drawa grill
* at (x ,y, z)
* 
*/

void grill(double x,double y,double z,
          double dx,double dy,double dz,
          double h)
{

  //stud(x ,y ,z, 1, 0.5, 2, 90, 1);
  // Grill bar
  stud(x ,y ,z, 0.1, 0.1, h, -90, 0,  2);

  // Grill box
  //base
  cube(x, y + h, z, dx, dy, dz, 0, 2); // dx = 0.9, dy = 0.003, dz = 1.3
  //myPoint(x , y + h, z + 1.2 );
  //right
  cube2(x - dx , y + h, z - dz , // position
        1.2, 1, dy,  // dimension
        0, 2);
  // left
  cube2(x - dx , y + h, z + dz , // position
        1.2, 1, dy,  // dimension
        0, 2);
  // back
  cube2(x - dx , y + h, z + dz , // position
        dz * 2, dx, dy,  // dimension
        90, 2);

  for (double i = -1.25 ; i < 1.5; i += 0.25)
  {
    //printf("===> %f\n", i);
    if( i == -1.25)
     {
      stud(x - dx , y + h + 0.5 ,i + z , 0.05, 0.05, dz * 1.5, 90, 1, 2); // grill right handle
     } 
    else if (i == 1.25)
    {
      stud(x - dx , y + h + 0.5 ,i + z, 0.05, 0.05, dz * 1.5, 90, 1, 2); // grill left handle
    }
    else
    {
      stud(x - dx , y + h + 0.5 ,i + z, 0.05, 0.05, dz * 1.1 , 90, 1, 2); // grill bars
    }
    
  }

  stud(x + (dx * dy)  , y + h + 0.5 ,0 + z - dz,
       0.05, 0.05, dz * 2 ,
       180, 2, 2); // grill frony bar

  stud(x - dx + 0.1 , y + h + 0.5 ,0 + z - dz,
       0.05, 0.05, dz * 2 ,
       180, 2, 2); // grill back bar

  
}

void pillar(double x,double y,double z,
          double dx,double dy,double dz)
{
  //pillars
  glColor3f(1,0,0);
  stud(x ,y ,z, dx, dy, dz, -90, 0,  0);
  glColor3f(0,0,1);
  for (double i = 1.5 ; i < 6; i += 2)
  {
    stud(x ,y + i ,z, dx*1.5, dy*1.5, dz/20, -90, 0,  0);
    stud(x ,y + i ,z, dx*1.5, dy*1.5, dz/20, -90, 0,  0);
    stud(x ,y + i ,z, dx*1.5, dy*1.5, dz/20, -90, 0,  0);
  }
}

void roof(double x,double y,double z,
          double dx,double dy,double dz,
          double d)
{
  double hight = dz;
   //  Save transformation
  glPushMatrix();
  glColor3f(0,1,0);
  glBegin(GL_TRIANGLES);

  glVertex3f(x,hight, z);
  glVertex3f(x + d ,hight, z);
  glVertex3f(x + d/2,hight+d/4, z+d/2);
  
  glVertex3f(x,hight, z);
  glVertex3f(x  ,hight, z + d);
  glVertex3f(x + d/2,hight+d/4, z+d/2);
  
  glVertex3f(x,hight, z + d);
  glVertex3f(x + d ,hight, z + d);
  glVertex3f(x + d/2,hight+d/4, z+d/2);

  glVertex3f(x + d,hight, z);
  glVertex3f(x + d ,hight, z + d);
  glVertex3f(x + d/2,hight+d/4, z+d/2);
  glEnd();
  //  Undo transformations
  glPopMatrix();
}

void playground_floor(double x,double y,double z,
          
          double d)
{

  double hight = dz*3.75;

  cube2(x,hight,z,    d, 0.25, d, 0,10);
}


void barrier(double x,double y,double z,
             double d, double angle, int axis)
{
  double hight = y + d/1.25;

  glColor3f(0,0,1);
  // horizantal bars
  stud(x,hight,z, 0.1,0.1,d, angle,1,0);
  stud(x,hight - 1,z, 0.1,0.1,d, angle,1,0);

  //vertical bars
  for (double i = 1 ; i < d; i += 1)
  {
    if(axis == 0)
    {
    stud(x + i ,hight - 1,z ,   0.1,0.1, 1, -90,0, 0);
    }
    else{

      stud(x ,hight - 1,z - i,   0.1,0.1, 1, -90,0, 0);
    }
    
  }
}

void slide(double x,double y,double z,
          double d)
{
   double hight = dz*3.9;

   cube3(x+d,hight,z-d+1.5,    d, 0.25, d-3, -40,10);
}


/*
* (x, y, z) location
* (dx, dy, dz) pillar size
* d = distance beterrn pillars
*
*/

void playground(double x,double y,double z,
          double dx,double dy,double dz,
          double d)
{
  
  //pillars
  // inbetween
  pillar(x,     y, z,      dx,dy,dz);
  pillar(x + d, y, z,      dx,dy,dz);
  barrier(x,y,z, d,90,0);

  // with roof
  pillar(x + d, y, z + d,  dx,dy,dz);
  pillar(x    , y, z + d,  dx,dy,dz);

  // with floor
  pillar(x + d, y, z - d,  dx,dy,dz);
  pillar(x    , y, z - d,  dx,dy,dz);
  barrier(x,y,z, d,180,1);

  // cylinderical stairs
  glColor3f(0,0,1);
  stud(x+2.25,y,z - d - 0.5,   1.3,1.3,3.8,   -90,0,0);
  glColor3f(1,0,0);
  stud(x+4.75,y,z - d - 1,   1.3,1.3,3.25,   -90,0,0);
  glColor3f(1,1,0);
  stud(x+2.5,y,z - d - 2,   1.3,1.3,2.5,   -90,0,0);
  glColor3f(0,1,0);
  stud(x+4.25,y,z - d - 3,   1.3,1.3,1.5,   -90,0,0);
  glColor3f(0,0,1);
  stud(x+2.5,y,z - d - 4,   1.3,1.3,0.5,   -90,0,0);
  // roof

  roof(x,y,z      ,dx,dy,dz,    d);

  playground_floor(x,y,z - d,d);


  //silde
  slide(x,y,z,d);
  
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

 // printf("D = %f\n", D);
  
  //double offset = 2.0;
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


  //  Light switch
  if (light)
  {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {12*Cos(zh),ylight,12*Sin(zh),1};
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
      // **** Project *****
      

    
// draw_blast();
  
        if(fireworks)
        {
          int i;
          //glClear(GL_COLOR_BUFFER_BIT);
          if (step < 0.9*length) {
          for (i=0; i<numPoints; i++)
          yacc[i] -= 0.02 / length; // gravity
          draw_blast();
          }
          step ++;
          if (step > length) fireworks_initialize();
        }

          //  Draw sky
          if (box) Sky(10*dim);
          picnicTable(1,0,1,2);
    
      grill(-3 ,0 ,-3,
            0.9, 0.003, 1.3,
            2.75);    
    playground(10,0,-10,    0.5,0.5,6, 7);
      // **** Project *****       
     // glCallList(obj);
//  myPoint(1,1,1);
      //cube(0,-10*dim,0 ,3,0.25,4, 0 ,3);
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
   Print("Angle=%d,%d  Dim=%.1f Light=%s Texture=%s  Mode = %s Skybox = %s Fireworks = %s",th,ph,dim,light?"On":"Off",mode?"Replace":"Modulate",text[displayMode], box?"On":"Off", fireworks?"On":"Off");
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
   else if (ch=='p' && box == 1)
    box = 0;
   else if (ch=='p' && box == 0)
    box = 1;
   else if (ch=='f' && fireworks == 1)
    fireworks = 0;
   else if (ch=='f' && fireworks == 0)
    fireworks = 1;
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
  //argv[0] = "bunny.obj";
  texture[0] = LoadTexBMP("PlasterBare.bmp");
  texture[1] = LoadTexBMP("WoodPlanksBare.bmp");
  texture[2] = LoadTexBMP("MetalFences0065.bmp");
   /*
  texture[3] = LoadTexBMP("BarkDecidious.bmp");
  texture[4] = LoadTexBMP("ivy.bmp");
  texture[5] = LoadTexBMP("door.bmp");
  texture[6] = LoadTexBMP("WindowsHouseOld.bmp");
  texture[7] = LoadTexBMP("Leaves0017.bmp");
  texture[8] = LoadTexBMP("Leaves0206.bmp");
*/
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

   //  Load object
   //obj = LoadOBJ(argv[0]);

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
