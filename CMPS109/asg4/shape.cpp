// Author: Sean McGrath, ssmcgrat@ucsc.edu
//
// $Id: shape.cpp,v 1.1 2015-07-16 16:47:51-07 - - $

#include <typeinfo>
#include <unordered_map>
#include <cmath>
using namespace std;

#include "shape.h"
#include "util.h"

static unordered_map<void*,string> fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

static unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape() {
   DEBUGF ('c', this);
}

text::text (void* glut_bitmap_font, const string& textdata):
      glut_bitmap_font(glut_bitmap_font), textdata(textdata) {
   DEBUGF ('c', this);
   
}

ellipse::ellipse (GLfloat width, GLfloat height):
dimension ({width, height}) {
   DEBUGF ('c', this);
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}


polygon::polygon (const vertex_list& vertices): vertices(vertices) {
   DEBUGF ('c', this);
}

rectangle::rectangle (GLfloat width, GLfloat height):
   polygon({ {0,0}, {width, 0}, {width, height}, {0, height} }) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");


}

square::square (GLfloat width): rectangle (width, width) {
   DEBUGF ('c', this);
}

diamond::diamond (GLfloat width, GLfloat height):
polygon({ {0,0}, {width/2,height/2}, {width,0}, {width/2, -height/2} }){

}

equilateral::equilateral (GLfloat width):
   polygon({ {0,0}, {width, 0}, {width/2, GLfloat(0.866025)*width}  }){ 
}

right_triangle::right_triangle(GLfloat width, GLfloat height):
   polygon({ {0,0}, {width, 0}, {0, height} }) { 

}

isosceles::isosceles(GLfloat width, GLfloat height):
   polygon({ {0,0}, {width,0}, {width/2, height} }) {

}

void text::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   
   glColor3ubv(color.ubvec);
   glRasterPos2f(center.xpos, center.ypos);
   void* font = glut_bitmap_font;
   for (auto ch: textdata) glutBitmapCharacter (font, ch);
}

void ellipse::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   
   glBegin(GL_POLYGON);
   glEnable (GL_LINE_SMOOTH);
   glColor3ubv (color.ubvec);
   const float DEG2RAD = M_PI / 180.0;
   for(int i =0; i<360; ++i){
      float rad = i * DEG2RAD;
      glVertex2f(cos(rad) * (dimension.xpos / 2) + center.xpos,
                 sin(rad) * (dimension.ypos / 2) + center.ypos);  
   } 
   glEnd();
}

void polygon::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glBegin(GL_POLYGON);
   glColor3ubv (color.ubvec);
   float xavg, yavg, denom;
   for (auto el : vertices){
      xavg += el.xpos;
      yavg += el.ypos;
      ++denom;
   }
   xavg = xavg / denom;
   yavg = yavg / denom;
   for (auto el : vertices){
      glVertex2f (center.xpos + el.xpos - xavg,
                  center.ypos + el.ypos - yavg);
   } 
   glEnd();
}

void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
   
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}

