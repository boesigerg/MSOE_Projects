/* This is an abstract base class representing a generic graphics
 * context.  Most implementation specifics will need to be provided by
 * a concrete implementation.  See header file for specifics. */

#define _USE_MATH_DEFINES	// for M_PI
#include <cmath>	// for trig functions
#include "gcontext.h"
#include "x11context.h"	

/*
 * Destructor - does nothing
 */
GraphicsContext::~GraphicsContext()
{
	// nothing to do
	// here to insure subclasses handle destruction properly
}


/* Fully functional line drawing function using Bresenhams line
 * algortihm and setPixel().
 * 
 * Parameters:
 *  x0, y0 - origin of line
 *  x1, y1 - end of line
 * 
 * Returns: void
 */
void GraphicsContext::drawLine(int x0, int y0, int x1, int y1){

  //draw_line(x0,y0,x1,y1);


  //Eliminate half of the octant possibilities
  //by forcing left to right line drawing
  if(x0>x1){
    int temp = x1;
    x1 = x0;
    x0 = temp;

    temp = y1;
    y1 = y0;
    y0 = temp;
  }

  //Get slope of line
  int dx = x1 - x0;
  int dy = y1 - y0;

  //Determine Octant and Draw
  float slope = (float) dy/dx;

  if(slope >= 1){           //Octant 1
    int d = 2*dx - dy;               //d0
    setPixel(x0, y0);                //start point
    int x = x0;
    for(int y = y0+1; y != y1; y++){ //iterate over y
      if(d<0){                       //Decision for di+1
	d = d + 2*dx;
      }else{
	d = d + 2*(dx-dy);
      }
      if(d>=0){                      //Decision for xi+1
	x++;
      }
      setPixel(x, y);
    }
    setPixel(x1, y1);               //end point

  }else if(slope >= 0){     //Octant 2 + Horizontal Line
    int d = 2*dy - dx;               //d0
    setPixel(x0, y0);                //start point
    int y = y0;
    for(int x = x0+1; x != x1; x++){ //iterate over x
      if(d<0){                       //Decision for di+1
	d = d + 2*dy;
      }else{
	d = d + 2*(dy-dx);
      }
      if(d>=0){                      //Decision for yi+1
	y++;
      }
      setPixel(x, y);
    }
    setPixel(x1, y1);               //end point

  }else if(slope >= -1){    //Octant 3
    dy*=-1;
    int d = 2*dy - dx;               //d0
    setPixel(x0, y0);                //start point
    int y = y0;
    for(int x = x0+1; x != x1; x++){ //iterate over x
      if(d<0){                       //Decision for di+1
	d = d + 2*dy;
      }else{
	d = d + 2*(dy-dx);
      }
      if(d>=0){                      //Decision for yi+1
	y--;
      }
      setPixel(x, y);
    }
    setPixel(x1, y1);               //end point

  }else{                    //Octant 4
    dy*=-1;
    int d = 2*dx - dy;               //d0
    setPixel(x0, y0);                //start point
    int x = x0;
    for(int y = y0+1; y != y1; y--){ //iterate over y
      if(d<0){                       //Decision for di+1
	d = d + 2*dx;
      }else{
	d = d + 2*(dx-dy);
      }
      if(d>=0){                      //Decision for xi+1
	x++;
      }
      setPixel(x, y);
    }
    setPixel(x1, y1);               //end point
  }

}



/* This is a naive implementation that uses floating-point math
 * and "setPixel" which will need to be provided by the concrete
 * implementation.
 * 
 * Parameters:
 * 	x0, y0 - origin/center of circle
 *  radius - radius of circle
 * 
 * Returns: void
 */
void GraphicsContext::drawCircle(int x0, int y0, unsigned int radius){

  //draw_circle(x0, y0, radius);

  int P = 1-radius;                 //P0
  int x = 0;                        //Starting at top of circle
  int y = radius;

  //Draw Initial Points
  setPixel(x0+radius, y0);          //Right
  setPixel(x0, y0+radius);          //Top
  setPixel(x0-radius, y0);          //Left
  setPixel(x0, y0-radius);          //Bottom

  while(x<=y){                      //Go through one octant of points
    x++;
    if(P<0){                        //Decision for Pi+1
      P = P + 2*x + 1;
    }else{
      P = P + 2*(x-y) + 1;
      y--;
    }

    //Draw point for each octant    __Octant__
    setPixel(x0+x, y0+y);              //1
    setPixel(x0+y, y0+x);              //2
    setPixel(x0+y, y0-x);              //3
    setPixel(x0+x, y0-y);              //4
    setPixel(x0-x, y0-y);              //5
    setPixel(x0-y, y0-x);              //6
    setPixel(x0-y, y0+x);              //7
    setPixel(x0-x, y0+y);              //8
  }

}

void GraphicsContext::endLoop()
{
	run = false;
}
