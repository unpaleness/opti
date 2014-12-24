#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include "method.h"

#define W 7

class GradientDescent : public Method
{
public:
  GradientDescent();
  ~GradientDescent();

  void init(double *); // init start point
  void count(int);

};

#endif // GRADIENTDESCENT_H
