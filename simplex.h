#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <cmath>

#include "method.h"

class Calc;

class Simplex : public Method
{
public:
  Simplex();
  ~Simplex();

  void init(double *); // three initial points, alpha, beta, gamma
  void count(int); //implements counting process

private:
  double _alpha;
  double _beta;
  double _gamma;

};

#endif // SIMPLEX_H
