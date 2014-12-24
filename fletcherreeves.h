#ifndef FLETCHERREEVES_H
#define FLETCHERREEVES_H

#include "method.h"

#define W 7

class FletcherReeves : public Method
{
public:
  FletcherReeves();
  ~FletcherReeves();

  void init(double *); // init start point
  void count(int);

};

#endif // FLETCHERREEVES_H
