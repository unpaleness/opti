#ifndef POWELL_H
#define POWELL_H

#include "method.h"

#define W 7

class Calc;

class Powell : public Method
{
public:
  Powell();
  ~Powell();

  void init(double *); // init start point
  void count(int);

private:

  int _rank(double **);
  void _gold(double *, double *, double *, double);

};

#endif // POWELL_H
