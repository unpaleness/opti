#ifndef FLETCHERREEVES_H
#define FLETCHERREEVES_H

#include "method.h"

#define W 7

class Calc;

class FletcherReeves : public Method
{
public:
  FletcherReeves();
  ~FletcherReeves();

  void init(double *); // init start point
  void count(int);

private:

  void _gold(double *, double *, double *, double);
};

#endif // FLETCHERREEVES_H
