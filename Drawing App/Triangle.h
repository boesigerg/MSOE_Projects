#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include "matrix.h"
#include "gcontext.h"
#include "x11context.h"
#include "shape.h"
#include "viewcontext.h"

using namespace std;

class Triangle : public Shape{

 public:

  Triangle();

  Triangle(int RGB);

  Triangle(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3);

  Triangle(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int RGB);

  Triangle(const Triangle& triangle);

  ~Triangle();

  Triangle& operator=(const Triangle& rhs);

  void draw(GraphicsContext* gc, ViewContext* vc);

  void drawFilled(GraphicsContext* gc, ViewContext* vc);

  std::ostream& out(std::ostream& os) const;

  std::istream& in(std::istream& is) const;

  Triangle* clone();

  int color;
  matrix origin;
  matrix vertices;

};
#endif
