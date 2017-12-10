#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <vector>
#include "gcontext.h"
#include "x11context.h"
#include "shape.h"
#include "Triangle.h"
#include "viewcontext.h"

using namespace std;

class Image{

 public:

  Image();

  Image(const Image& img);

  ~Image();

  Image& operator=(const Image& rhs);

  void add(Shape& shape);

  void draw(GraphicsContext* gc, ViewContext* vc);

  void drawFilled(GraphicsContext* gc, ViewContext* vc);

  std::ostream& out(std::ostream& os) const;

  std::istream& in(std::istream& is) const;

  void erase();

  vector<Shape*> image;

};
#endif
