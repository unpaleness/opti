#include <cmath>

#include "powell.h"
#include "calc.h"

Powell::Powell()
{
  _nPoints = 0;
  _points = new double *[POWELL_MAX];
  for(int i = 0; i < POWELL_MAX; i++)
    _points[i] = new double [3];
}

Powell::~Powell()
{
  for(int i = 0; i < POWELL_MAX; i++)
    delete [] _points[i];
  delete [] _points;
}

int Powell::nPoints() { return _nPoints; }

double Powell::points(int i, int coord) { return _points[i][coord]; }

void Powell::init(double *params)
{
  _x0[0] = params[0];
  _x0[1] = params[1];
}

//0 - minimum, 1 - maximum
void Powell::count(int extremum)
{
  double m = 1.0;
  if(extremum) // if maximum z inverts =)
    m = - 1.0;
}
