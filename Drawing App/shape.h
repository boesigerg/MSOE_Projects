#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include "matrix.h"
#include "gcontext.h"
#include "x11context.h"
#include "viewcontext.h"

using namespace std;

class Shape{

 public:

  Shape();

  Shape(int RGB);

  Shape(int x, int y, int z);

  Shape(int x, int y, int z, int RGB);

  Shape(const Shape& shape);

  virtual ~Shape();

  virtual void draw(GraphicsContext* gc, ViewContext* vc) = 0;

  virtual void drawFilled(GraphicsContext* gc, ViewContext* vc) = 0;

  virtual std::ostream& out(std::ostream& os) const;

  virtual std::istream& in(std::istream& is) const;

  virtual Shape* clone() = 0;

  int color;
  matrix origin;
  matrix vertices;

  Shape& operator=(const Shape& rhs);
};
#endif
