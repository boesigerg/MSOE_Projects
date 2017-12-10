#include "gcontext.h"
#include "x11context.h"
#include "shape.h"
#include "Triangle.h"
#include "Image.h"
#include "viewcontext.h"
#include <iostream>
#include <vector>

using namespace std;

Image::Image(){
}

Image::Image(const Image& img){
  for(int i=0; i<img.image.size(); i++){
    Shape* shape = img.image[i]->clone();
    this->image.push_back(shape);
  }
}

Image::~Image(){
  for(int i=0; i<this->image.size(); i++){
    delete this->image[i];
  }
  this->image.clear();
}

Image& Image::operator=(const Image& rhs){
  if(this != &rhs){
    this->~Image();
    for(int i=0; i<rhs.image.size(); i++){
      Shape* shape = rhs.image[i]->clone();
      this->image.push_back(shape);
    }
    return *this;
  }
}

void Image::add(Shape& shape){
  this->image.push_back(shape.clone());
}

void Image::draw(GraphicsContext *gc, ViewContext *vc){
  for(int i=0; i<this->image.size(); i++){
    this->image[i]->draw(gc,vc);
  }
}

void Image::drawFilled(GraphicsContext *gc, ViewContext *vc){
  for(int i=0; i<this->image.size(); i++){
    image[i]->drawFilled(gc,vc);
  }
}

std::ostream& Image::out(std::ostream& os) const{
  for(int i=0; i<this->image.size(); i++){
    image[i]->out(std::cout);
  }
}

std::istream& Image::in(std::istream& is) const{
  for(int i=0; i<this->image.size(); i++){
    image[i]->in(std::cin);
  }
}

void Image::erase(){
  for(int i=0; i<this->image.size(); i++){
    delete image[i];
  }
}
