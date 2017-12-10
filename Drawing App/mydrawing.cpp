#include "mydrawing.h"
#include "x11context.h"
#include "Image.h"
#include "gcontext.h"
#include "shape.h"
#include "Triangle.h"
#include <iostream>

MyDrawing::MyDrawing(GraphicsContext* gc){
  dragging = false;
  keyPressed = false;
  drawSetting = DRAW_POINT;
  currentColor = GraphicsContext::WHITE;
  vc = ViewContext(800,600); //dont know how to get window dimensions
  Triangle xaxis(0,0,0,300,0,0,0,0,0,GraphicsContext::BLUE);
  Triangle yaxis(0,0,0,0,300,0,0,0,0,GraphicsContext::RED);
  Triangle zaxis(0,0,0,0,0,300,0,0,0,GraphicsContext::GREEN);
  drawnPic.add(xaxis);
  drawnPic.add(yaxis);
  drawnPic.add(zaxis);
}

void MyDrawing::paint(GraphicsContext* gc){
  drawnPic.draw(gc,&vc);           //Draw each shape
}

void MyDrawing::keyDown(GraphicsContext* gc, unsigned int keycode){
  if(!dragging){
    keyPressed = true;              //Prevent other button press events
    switch(keycode){
    case 'p':   //P
      //Draw Setting: Point
      drawSetting = DRAW_POINT;
      break;
    case 'l':   //L
      //Draw Setting: Line
      drawSetting = DRAW_LINE;
      break;
    case 't':   //T
      //Draw Setting: Triangle
      drawSetting = DRAW_TRIANGLE_P1;
      break;
    case 's':   //S
      //Save image
      break;

      //1-9 choose color of line
    case 49:   //1
      gc->setColor(GraphicsContext::BLACK);
      currentColor = GraphicsContext::BLACK;
      break;
    case 50:   //2
      gc->setColor(GraphicsContext::GRAY);
      currentColor = GraphicsContext::GRAY;
      break;
    case 51:   //3
      gc->setColor(GraphicsContext::WHITE);
      currentColor = GraphicsContext::WHITE;
      break;
    case 52:   //4
      gc->setColor(GraphicsContext::RED);
      currentColor = GraphicsContext::RED;
      break;
    case 53:   //5
      gc->setColor(GraphicsContext::YELLOW);
      currentColor = GraphicsContext::YELLOW;
      break;
    case 54:   //6
      gc->setColor(GraphicsContext::GREEN);
      currentColor = GraphicsContext::GREEN;
      break;
    case 55:   //7
      gc->setColor(GraphicsContext::CYAN);
      currentColor = GraphicsContext::CYAN;
      break;
    case 56:   //8
      gc->setColor(GraphicsContext::BLUE);
      currentColor = GraphicsContext::BLUE;
      break;
    case 57:   //9
      gc->setColor(GraphicsContext::MAGENTA);
      currentColor = GraphicsContext::MAGENTA;
      break;
    case 'u':  //U
      //Translate Up
      gc->clear();            //Clear drawing
      vc.panUp();             //adjust transform matrix
      drawnPic.draw(gc,&vc);  //Redraw picture with new tranform matrix
      break;
    case 'j':  //J
      //Translate Down
      gc->clear();
      vc.panDown();
      drawnPic.draw(gc,&vc);
      break;
    case 'k':  //K
      //Translate Right
      gc->clear();
      vc.panRight();
      drawnPic.draw(gc,&vc);
      break;
    case 'h':  //H
      //Translate Left
      gc->clear();
      vc.panLeft();
      drawnPic.draw(gc,&vc);
      break;
    case ',':  //<
      //Rotate Counter-Clockwise
      gc->clear();
      vc.rotateCCW();
      drawnPic.draw(gc,&vc);
      break;
    case '.':  //>
      //Rotate Clockwise
      gc->clear();
      vc.rotateCW();
      drawnPic.draw(gc,&vc);
      break;
    case '=':  //+
      //Zoom In
      gc->clear();
      vc.zoomIn();
      drawnPic.draw(gc,&vc);
      break;
    case '-':  //-
      //Zoom Out
      gc->clear();
      vc.zoomOut();
      drawnPic.draw(gc,&vc);
      break;
    case 'r': //R
      //Reset Transformations
      gc->clear();
      vc.reset(gc);
      drawnPic.draw(gc,&vc);
      break;
    default:
      break;
    }
  }
}

void MyDrawing::keyUp(GraphicsContext* gc, unsigned int keycode){
  if(!dragging){
    keyPressed = false;      //Allow for other button presses
  }
}

void MyDrawing::mouseButtonDown(GraphicsContext* gc, unsigned int button, int x, int y){
  if(!keyPressed){
    if(button == 1){
      gc->setMode(GraphicsContext::MODE_XOR);   //For rubberbanding
      switch(drawSetting){
      case DRAW_POINT:
	break;
      case DRAW_LINE:
	x0 = x1 = x;                            //Initialize points
	y0 = y1 = y;
	gc->drawLine(x0,y0,x1,y1);              //Start rubberbanding at mouse location
	break;
      case DRAW_TRIANGLE_P1:                    //First line of triangle
	x0 = x1 = x;
	y0 = y1 = y;
	gc->drawLine(x0,y0,x1,y1);
	break;
      case DRAW_TRIANGLE_P2:                    //Other two lines of triangle
	x2 = x;
	y2 = y;
	gc->drawLine(x0,y0,x2,y2);
	gc->drawLine(x1,y1,x2,y2);
	break;
      default:
	break;
      }
    }else if(button == 3){
      x0 = x;                                   //Store where panning started
      y0 = y;
      x2 = drawSetting;                         //Store current draw setting in unneeded variable for later
      drawSetting = PANNING;                    //Panning mode while right clicked
    }else if(button == 4){
      //Zoom in on mouse
      gc->clear();                              //Clear image
      vc.zoomInMouse(x,y);                      //Recalculate transformation matrix about cursor
      drawnPic.draw(gc,&vc);                    //Redraw image
      x2 = drawSetting;                         //Store current drawSetting
      drawSetting = SCROLLING;
    }else if(button == 5){
      //Zoom Out on mouse
      gc->clear();
      vc.zoomOutMouse(x,y);
      drawnPic.draw(gc,&vc);
      x2 = drawSetting;
      drawSetting = SCROLLING;
    }else if(button == 8 || button == 9){
      //Rotate with mouse
      x0 = x;                                   //store intial y coord
      x2 = drawSetting;                         //store current draw setting
      drawSetting = ROTATING;
    }
    dragging = true;
  }
}

void MyDrawing::mouseButtonUp(GraphicsContext* gc, unsigned int button, int x, int y){
  if(dragging){
    switch(drawSetting){
    case DRAW_POINT:
      {
	matrix deviceCoords(4,4);            //get points to transform
	deviceCoords[0][0] = x;
	deviceCoords[0][1] = y;
	deviceCoords[0][2] = 0;
	deviceCoords[1][0] = 0;
	deviceCoords[1][1] = 0;
	deviceCoords[1][2] = 0;
	deviceCoords[2][0] = 0;
	deviceCoords[2][1] = 0;
	deviceCoords[2][2] = 0;
	matrix modelCoords = vc.deviceToModel(deviceCoords); //convert to model coords
	x0 = modelCoords[0][0];
	y0 = modelCoords[0][1];
	z0 = modelCoords[0][2];
	x1 = modelCoords[1][0];
	y1 = modelCoords[1][1];
	z1 = modelCoords[1][2];
	x2 = modelCoords[2][0];
	y2 = modelCoords[2][1];
	z2 = modelCoords[2][2];
	Triangle shape(x0,y0,z0,x1,y1,z1,x2,y2,z2,currentColor);
	gc->setMode(GraphicsContext::MODE_NORMAL);           //No longer rubberbanding
	gc->setPixel(x,y);
	drawnPic.add(shape);
	break;
      }
    case DRAW_LINE:
      {
	gc->drawLine(x0,y0,x1,y1);          //Undraw last rubberbanding line
	matrix deviceCoords(4,4);           //get points to transform
	deviceCoords[0][0] = x0;
	deviceCoords[0][1] = y0;
	deviceCoords[0][2] = 0;
	deviceCoords[1][0] = x1;
	deviceCoords[1][1] = y1;
	deviceCoords[1][2] = 0;
	deviceCoords[2][0] = 0;
	deviceCoords[2][1] = 0;
	deviceCoords[2][2] = 0;
	gc->setMode(GraphicsContext::MODE_NORMAL);           //no longer rubberbanding
	gc->drawLine(x0,y0,x1,y1);                           //Draw actual line
	matrix modelCoords = vc.deviceToModel(deviceCoords); //Convert points for storage
	x0 = modelCoords[0][0];
	y0 = modelCoords[0][1];
	z0 = modelCoords[0][2];
	x1 = modelCoords[1][0];
	y1 = modelCoords[1][1];
	z1 = modelCoords[1][2];
	x2 = modelCoords[2][0];
	y2 = modelCoords[2][1];
	z2 = modelCoords[2][2];
	Triangle shape(x0,y0,z0,x1,y1,z1,x2,y2,z2,currentColor);
	drawnPic.add(shape);
	break;
      }
    case DRAW_TRIANGLE_P1:                //Finalize first line of triangle
      gc->drawLine(x0,y0,x1,y1);
      x1 = x;
      y1 = y;
      gc->drawLine(x0,y0,x1,y1);
      drawSetting = DRAW_TRIANGLE_P2;     //Next mouseDown() will finish triangle
      break;
    case DRAW_TRIANGLE_P2:
      {
	gc->drawLine(x0,y0,x2,y2);        //Undraw last rubberbanding
	gc->drawLine(x1,y1,x2,y2);
	matrix deviceCoords(4,4);         //get points to transform
	deviceCoords[0][0] = x0;
	deviceCoords[0][1] = y0;
	deviceCoords[0][2] = 0;
	deviceCoords[1][0] = x1;
	deviceCoords[1][1] = y1;
	deviceCoords[1][2] = 0;
	deviceCoords[2][0] = x;
	deviceCoords[2][1] = y;
	deviceCoords[2][2] = 0;
	gc->setMode(GraphicsContext::MODE_NORMAL);              //No longer rubberbanding
	gc->drawLine(x0,y0,x,y);                                //Finish drawing triangle
	gc->drawLine(x1,y1,x,y);
	matrix modelCoords = vc.deviceToModel(deviceCoords);    //Convert points for storage
	x0 = modelCoords[0][0];
	y0 = modelCoords[0][1];
	z0 = modelCoords[0][2];
	x1 = modelCoords[1][0];
	y1 = modelCoords[1][1];
	z1 = modelCoords[1][2];
	x2 = modelCoords[2][0];
	y2 = modelCoords[2][1];
	z2 = modelCoords[2][2];
	Triangle shape(x0,y0,z0,x1,y1,z1,x2,y2,z2,currentColor);
	drawnPic.add(shape);
	drawSetting = DRAW_TRIANGLE_P1;                         //Next mouseDown will be to start new triangle
	break;
      }
    case PANNING:
      drawSetting = x2;                   //Restore old drawSetting
      break;
    case SCROLLING:
      drawSetting = x2;                   //Restore old drawSetting
      break;
    case ROTATING:
      drawSetting = x2;                   //Restore old drawSetting
      break;
    default:
      break;
    }
    dragging=false;
  }
}

void MyDrawing::mouseMove(GraphicsContext* gc, int x, int y){
  if(dragging){
    switch(drawSetting){
    case DRAW_POINT:
      break;
      //Redraw rubberbanding line for every mouse movement
    case DRAW_LINE:
      gc->drawLine(x0,y0,x1,y1);
      x1 = x;
      y1 = y;
      gc->drawLine(x0,y0,x1,y1);
      break;
    case DRAW_TRIANGLE_P1:
      gc->drawLine(x0,y0,x1,y1);
      x1 = x;
      y1 = y;
      gc->drawLine(x0,y0,x1,y1);
      break;
    case DRAW_TRIANGLE_P2:
      gc->drawLine(x0,y0,x2,y2);
      gc->drawLine(x1,y1,x2,y2);
      x2 = x;
      y2 = y;
      gc->drawLine(x0,y0,x2,y2);
      gc->drawLine(x1,y1,x2,y2);
      break;
    case PANNING:
      //Redraw image for new cursor location
      gc->clear();
      vc.panMouse(x-x0, y-y0);
      x0 = x;
      y0 = y;
      drawnPic.draw(gc,&vc);
      break;
    case ROTATING:
      //Rotate based on y movement
      gc->clear();
      vc.rotateMouse(x-x0);
      x0 = x;
      drawnPic.draw(gc,&vc);
      break;
    default:
      break;
    }
  }
}
