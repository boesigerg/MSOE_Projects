#include "shape.h"
#include "matrix.h"
#include "gcontext.h"
#include "x11context.h"
#include <iostream>

using namespace std;
  
Shape::Shape(){
  this->color = GraphicsContext::WHITE;
  this->origin = matrix(3, 1);
  this->vertices = matrix(3,3);
}

Shape::Shape(int RGB){
  this->color = RGB;
  this->origin = matrix(3, 1);
  this->vertices = matrix(3,3);
}

Shape::Shape(int x, int y, int z){
  this->color = GraphicsContext::WHITE;
  this->origin = matrix(3, 1);
  this->origin[0][0]=x;
  this->origin[0][1]=y;
  this->origin[0][2]=z;
  this->vertices = matrix(3,3);
}

Shape::Shape(int x, int y, int z, int RGB){
  this->color = RGB;
  this->origin = matrix(3, 1);
  this->origin[0][0]=x;
  this->origin[0][1]=y;
  this->origin[0][2]=z;
  this->vertices = matrix(3,3);
}

Shape::Shape(const Shape& shape):color(shape.color),origin(shape.origin), vertices(shape.vertices){
}

Shape::~Shape(){
}

Shape& Shape::operator=(const Shape& rhs){
  this->color = rhs.color;
  this->origin = rhs.origin;
}

ostream& Shape::out(ostream& os) const{
  os << color << endl;
  os << origin << endl;
  os << vertices << endl;
}

istream& Shape::in(istream& is) const{
  //is >> color;
  //is >> origin;
}
