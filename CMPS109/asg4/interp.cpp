// Author: Sean McGrath, ssmcgrat@ucsc.edu
//
// $Id: interp.cpp,v 1.2 2015-07-16 16:57:30-07 - - $

#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"
#include "graphics.h"

unordered_map<string,interpreter::interpreterfn>
interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
   {"moveby" , &interpreter::moveby    },
   {"border" , &interpreter::border    },
};

unordered_map<string,interpreter::factoryfn>
interpreter::factory_map {
   {"text"       , &interpreter::make_text       },
   {"ellipse"    , &interpreter::make_ellipse    },
   {"circle"     , &interpreter::make_circle     },
   {"polygon"    , &interpreter::make_polygon    },
   {"rectangle"  , &interpreter::make_rectangle  },
   {"square"     , &interpreter::make_square     },
   {"diamond"    , &interpreter::make_diamond    },
   {"triangle"   , &interpreter::make_polygon    },
   {"equilateral", &interpreter::make_equilateral},
   {"right_triangle", &interpreter::make_right_triangle },
   {"isosceles"  , &interpreter::make_isosceles  },
};

static unordered_map<string, void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

interpreter::shape_map interpreter::objmap;

interpreter::~interpreter() {
   for (const auto& itor: objmap) {
      cout << "objmap[" << itor.first << "] = "
           << *itor.second << endl;
   }
}

void interpreter::interpret (const parameters& params) {
   DEBUGF ('i', params);
   //cout << "interpreter::interpret()" << endl;
   param begin = params.cbegin();
   string command = *begin;
   auto itor = interp_map.find (command);
   if (itor == interp_map.end()){//throw runtime_error ("syntax error");
      cerr << "syntax error" << endl;
      return;
   }
   interpreterfn func = itor->second;
   func (++begin, params.cend());
}

void interpreter::do_define (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string name = *begin;
   objmap.emplace (name, make_shape (++begin, end));
}


void interpreter::do_draw (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   //cout << "do_draw" << endl;
   if (end - begin != 4){ // throw runtime_error ("syntax error");
      cerr << "syntax error" << endl;
      return;
   }
   string colour = begin[0];
   shape_map::const_iterator itor = objmap.find (begin[1]);
   if (itor == objmap.end()) {
      //throw runtime_error (begin[1] + ": no such shape");
      cerr << begin[1] << ": no such shape" << endl;
      return;
   }
   
   rgbcolor color {colour};
   
   vertex where {from_string<GLfloat> (begin[2]),
                 from_string<GLfloat> (begin[3])};
   object obj(itor->second, where, color);
   window::push_back(obj);
   
}

shape_ptr interpreter::make_shape (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   //cout << "interpreter::make_shape" << endl;
   string type = *begin++;
   auto itor = factory_map.find(type);
   if (itor == factory_map.end()) {
      throw runtime_error (type + ": no such shape");
   }
   factoryfn func = itor->second;
   return func (begin, end);
}

shape_ptr interpreter::make_text (param begin, param end) {
   DEBUGF ('f', range (begin, end)); 
   //cout << "interpreter::make_text" << endl;
   string font = *begin++;
   string words = "";
   string space = "";
   while(begin != end){
      words += space + *begin;
      ++begin;
      space = " ";
   }
      
   //void* fontcode = GLUT_BITMAP_8_BY_13;
   auto itor = fontcode.find(font);
   if (itor == fontcode.end()){// throw runtime_error ("syntax error");
      cerr << "syntax error" << endl;
      return nullptr;
   }
   return make_shared<text> (itor->second, words);
}

shape_ptr interpreter::make_ellipse (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   
   string width = *begin++;
   string height = *begin;
   int w = stoi(width);
   int h = stoi(height);
   return make_shared<ellipse> (GLfloat(w), GLfloat(h));
}

shape_ptr interpreter::make_circle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   int diameter = stoi(*begin);
   return make_shared<circle> (GLfloat(diameter));
}

shape_ptr interpreter::make_polygon (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list vl;
   vertex v;
   int xpos, ypos;
   while(begin != end){
      xpos = stoi(*begin++);
      ypos = stoi(*begin++);
      GLfloat x = GLfloat(xpos);
      GLfloat y = GLfloat (ypos);
      v = {x, y};
      vl.push_back(v);   
   }
   
   return make_shared<polygon> (vl);
}

shape_ptr interpreter::make_rectangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   int width = stoi(*begin++);
   int height = stoi(*begin);
   return make_shared<rectangle> (GLfloat(width), GLfloat(height));
}

shape_ptr interpreter::make_square (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   int width = stoi(*begin);
   return make_shared<square> (GLfloat(width));
}

shape_ptr interpreter::make_diamond (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   int width = stoi(*begin++);
   int height = stoi(*begin);
   return make_shared<diamond> (GLfloat(width), GLfloat(height));
}

shape_ptr interpreter::make_equilateral (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   int width = stoi(*begin);
   return make_shared<equilateral> (GLfloat(width)); 
}

shape_ptr interpreter::make_right_triangle (param begin, param end){
   DEBUGF ('f', range (begin, end));
   int width = stoi(*begin++);
   int height = stoi(*begin);
   return make_shared<right_triangle> (GLfloat(width),
                                         GLfloat(height));
}

shape_ptr interpreter::make_isosceles (param begin, param end){
   DEBUGF ('f', range (begin, end));
   int width = stoi(*begin++);
   int height = stoi(*begin);
   return make_shared<isosceles> (GLfloat(width), GLfloat(height));
}

void interpreter::moveby(param begin, param end){
   DEBUGF ('f', range (begin, end));
   int pix = stoi(*begin);
   window::setmoveby(pix);
}

void interpreter::border(param begin, param end){
   DEBUGF ('f', range (begin, end));
   
}
