#ifndef POWELL_H
#define POWELL_H

#include "method.h"

#define POWELL_MAX 100

class Calc;

class Powell : public Method
{
public:
  Powell();
  ~Powell();

  int nPoints();
  double points(int, int);
  void init(double *); // init start point
  void count(int);

private:
  int _nPoints; // amount of points
  double **_points; // 3D-points of searching way
  double _x0[2]; // start point

};

#endif // POWELL_H
