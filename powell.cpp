#include <cmath>
#include <iostream>

using namespace std;

#include "powell.h"
#include "calc.h"

Powell::Powell()
{
  _nPoints = 1;
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
  _points[0][0] = params[0];
  _points[0][1] = params[1];
}

// 0 - minimum, 1 - maximum
void Powell::count(int extremum)
{
  int it;
  double m = 2.0 * extremum - 1.0;
  double y[4][2]; // points
  double **x = _points;
  // initializing
  x[0][2] = m * _calc->f(x[0][0], x[0][1]);
  for(it = 0; it < POWELL_MAX - 1; it++)
  {
    cout << it + 1 << '\n';
    // 0: minimizing by y
    y[0][0] = x[it][0];
    y[0][1] = x[it][1];
    y[1][0] = y[0][0];
    y[1][1] = _minimum(1, y[1][0], m);
    // 1: minimizing by x
    y[2][1] = y[1][1];
    y[2][0] = _minimum(0, y[2][1], m);
    x[it + 1][0] = y[2][0];
    x[it + 1][1] = y[2][1];
    x[it + 1][2] = m * _calc->f(x[it + 1][0], x[it + 1][1]);
    // checking for vector length
    if(sqrt((x[it + 1][0] - x[it][0]) *
            (x[it + 1][0] - x[it][0]) +
            (x[it + 1][1] - x[it][1]) *
            (x[it + 1][1] - x[it][1])) < _e)
    {
      break;
    }
  }
  _nPoints = it + 1;
}

// minimum with other variable fixed
// gold section
double Powell::_minimum(int iVar, double otherVar, int m)
{
  if(iVar == 0)
    cout << "Seeking minimum with y = " << otherVar << '\n';
  else
    cout << "Seeking minimum with x = " << otherVar << '\n';
  int limit = 100;
  double a;
  double b;
  double p1;
  double p2;
  double f1;
  double f2;
  a = _calc->min(iVar);
  b = _calc->max(iVar);
  for(int i = 0; i < limit; i++)
  {
//    cout << ' ' << i << '\n';
    p1 = a + (3.0 - sqrt(5.0)) * (b - a) / 2.0;
    p2 = a + b - p1;
    if(iVar == 0)
    {
      f1 = m * _calc->f(p1, otherVar);
      f2 = m * _calc->f(p2, otherVar);
    }
    else
    {
      f1 = m * _calc->f(otherVar, p1);
      f2 = m * _calc->f(otherVar, p2);
    }
//    cout << " p1 = " << p1 << '\n';
//    cout << " p2 = " << p2 << '\n';
    if(f1 < f2) b = p2;
    else a = p1;
    if(abs(p1 - p2) < _e)
      break;
  }
  if(iVar == 0)
    cout << "x_min = " << (p1 + p2) / 2.0 << '\n';
  else
    cout << "y_min = " << (p1 + p2) / 2.0 << '\n';
  return (p1 + p2) / 2.0;
}
