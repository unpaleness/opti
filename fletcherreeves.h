#ifndef FLETCHERREEVES_H
#define FLETCHERREEVES_H

#include "method.h"

#define FLETCHER_MAX 100
#define W 7

class Calc;

class FletcherReeves : public Method
{
public:
  FletcherReeves();
  ~FletcherReeves();

  int nPoints();
  double points(int, int);
  void init(double *); // init start point
  void count(int);

private:
  int _nPoints; // amount of points
  double **_points; // 3D-points of searching way

  void _gold(double *, double *, double *, double);
};

#endif // FLETCHERREEVES_H
