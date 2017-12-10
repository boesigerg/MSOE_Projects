#include "matrix.h"
#include "Triangle.h"
#include "gcontext.h"
#include "x11context.h"
#include "shape.h"
#include "viewcontext.h"
#include <math.h>
#include <iostream>

using namespace std;

Triangle::Triangle(){
  this->color = GraphicsContext::WHITE;
  this->vertices = matrix(4,4);
}

Triangle::Triangle(int RGB){
  this->color = RGB;
  this->vertices = matrix(4,4);
}

Triangle::Triangle(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3){
  this->color = GraphicsContext::WHITE;

  this->origin = matrix(3, 1);
  this->origin[0][0]=(x1+x2+x3)/3;
  this->origin[0][1]=(y1+y2+y3)/3;
  this->origin[0][2]=(z1+z2+z3)/3;

  this->vertices = matrix(4,4);
  this->vertices[0][0]=x1;
  this->vertices[0][1]=y1;
  this->vertices[0][2]=1;
  this->vertices[1][0]=x2;
  this->vertices[1][1]=y2;
  this->vertices[1][2]=1;
  this->vertices[2][0]=x3;
  this->vertices[2][1]=y3;
  this->vertices[2][2]=1;
}

Triangle::Triangle(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int RGB){
  this->color=RGB;

  this->origin = matrix(3, 1);
  this->origin[0][0]=(x1+x2+x3)/3;
  this->origin[0][1]=(y1+y2+y3)/3;
  this->origin[0][2]=(z1+z2+z3)/3;

  this->vertices = matrix(4,4);
  this->vertices[0][0]=x1;
  this->vertices[0][1]=y1;
  this->vertices[0][2]=z1;
  this->vertices[0][3]=1;
  this->vertices[1][0]=x2;
  this->vertices[1][1]=y2;
  this->vertices[1][2]=z2;
  this->vertices[1][3]=1;
  this->vertices[2][0]=x3;
  this->vertices[2][1]=y3;
  this->vertices[2][2]=z3;
  this->vertices[2][3]=1;
  this->vertices[3][0]=0;
  this->vertices[3][1]=0;
  this->vertices[3][2]=0;
  this->vertices[3][3]=1;
}

Triangle::Triangle(const Triangle& triangle): color(triangle.color), origin(triangle.origin), vertices(triangle.vertices){
}

Triangle::~Triangle(){
}

Triangle& Triangle::operator=(const Triangle& triangle){
  this->color = triangle.color;
  this->origin = triangle.origin;
  this->vertices = triangle.vertices;
}

void Triangle::draw(GraphicsContext *gc, ViewContext *vc){
  matrix deviceCoords = vc->modelToDevice(this->vertices);
  int x1 = deviceCoords[0][0];
  int y1 = deviceCoords[0][1];
  int x2 = deviceCoords[1][0];
  int y2 = deviceCoords[1][1];
  int x3 = deviceCoords[2][0];
  int y3 = deviceCoords[2][1];

  gc->setColor(this->color);
  gc->drawLine(x1, y1, x2, y2);
  gc->drawLine(x2, y2, x3, y3);
  gc->drawLine(x3, y3, x1, y1);
}

void Triangle::drawFilled(GraphicsContext *gc, ViewContext *vc){

  this->draw(gc,vc);
  matrix deviceCoords = vc->modelToDevice(this->vertices);

  
  /*Sort Triangle Vertices by X-Coordinate*/
  int high, med, low; //Holds order of vertices | Number corresponds to matrix column
  if(deviceCoords[0][0]>deviceCoords[1][0]){
    if(deviceCoords[0][0]>deviceCoords[2][0]){
      high = 0;
      if(deviceCoords[1][0]>deviceCoords[2][0]){
	med = 1;
	low = 2;
      }else{
	med = 2;
	low = 1;
      }
    }else{
      high = 2;
      med = 0;
      low = 1;
    }
  }else{
    if(deviceCoords[1][0]>deviceCoords[2][0]){
      high = 1;
      if(deviceCoords[0][0]>deviceCoords[2][0]){
	med = 0;
	low = 2;
      }else{
	med = 2;
	low = 0;
      }
    }else{
      high = 2;
      med = 1;
      low = 0;
    }
  }

  /*Scan Line Fill left -> right*/
  //Find slope of each edge
  float slopeA = (float) ((deviceCoords[med][1]-deviceCoords[low][1])/(deviceCoords[med][0]-deviceCoords[low][0]));
  float slopeB = (float) ((deviceCoords[high][1]-deviceCoords[med][1])/(deviceCoords[high][0]-deviceCoords[med][0]));
  float slopeC = (float) ((deviceCoords[high][1]-deviceCoords[low][1])/(deviceCoords[high][0]-deviceCoords[low][0]));

  if(!isinf(slopeC)){ //If slopeC=inf, triangle is really just a vertical line, no need to fill
    int x;        //Used to iterate through triangle
    float y1, y2; // y1 for a and b edges, y2 for c edge
    x = deviceCoords[low][0];
    y1 = deviceCoords[low][1];
    y2 = y1;

    if(isinf(slopeA)){ //If slopeA=inf, First while will be skipped, immediately change y1 to breakpoint
      y1 = deviceCoords[med][1];
    }

    //Begin iteration. Draw vertical lines from edge A to C for each x coordinate
    while(x<deviceCoords[med][0]){
      x++;
      y1+=slopeA;
      y2+=slopeC;
      gc->drawLine(x,(int)y1,x,(int)y2);
    }
    //Breakpoint hit. Switch to edges B and C.
    while(x<deviceCoords[high][0]){
      x++;
      y1+=slopeB;
      y2+=slopeC;
      gc->drawLine(x,(int)y1,x,(int)y2);
    }
  }
}

ostream& Triangle::out(ostream& os) const{
  os << color << endl;
  os << origin << endl;
  os << vertices << endl;
}

istream& Triangle::in(istream& is) const{
}

Triangle* Triangle::clone(){
  return new Triangle(*this);
}
