#include "x11context.h"
#include <unistd.h>
#include <iostream>
#include "mydrawing.h"

int main(void){
  GraphicsContext* gc = new X11Context(800,600,GraphicsContext::BLACK);
  MyDrawing md(gc);
  gc->runLoop(&md);
  delete gc;
}
