#ifndef MYDRAWING_H
#define MYDRAWING_H

#include "drawbase.h"
#include "gcontext.h"
#include "Image.h"
#include "x11context.h"
#include "shape.h"
#include "Triangle.h"

//forward reference
class GraphicsContext;

class MyDrawing : public DrawingBase{

 public:
  MyDrawing(GraphicsContext* gc);
  virtual void paint(GraphicsContext* gc);
  virtual void keyDown(GraphicsContext* gc, unsigned int keycode);
  virtual void keyUp(GraphicsContext* gc, unsigned int keycode);
  virtual void mouseButtonDown(GraphicsContext* gc, unsigned int button, int x, int y);
  virtual void mouseButtonUp(GraphicsContext* gc, unsigned int button, int x, int y);
  virtual void mouseMove(GraphicsContext* gc, int x, int y);

 private:
  Image drawnPic;
  ViewContext vc;
  bool dragging; //flag to know if we are dragging
  bool keyPressed; //flag to know if a key event is occurring
  int drawSetting;
  static const int DRAW_POINT = 1;
  static const int DRAW_LINE = 2;
  static const int DRAW_TRIANGLE_P1 = 3;
  static const int DRAW_TRIANGLE_P2 = 4;
  static const int PANNING = 5;
  static const int SCROLLING = 6;
  static const int ROTATING = 7;
  int currentColor;
  int x0, y0, z0, x1, y1, z1, x2, y2, z2;
};
#endif
