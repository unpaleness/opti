#ifndef POWELL_H
#define POWELL_H

#include "method.h"

#define W 7

class Powell : public Method
{
public:
  Powell();
  ~Powell();

  void init(double *); // init start point
  void count(int);

private:

  int _rank(double **);

};

#endif // POWELL_H
