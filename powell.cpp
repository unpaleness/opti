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

// 1 - minimum, 0 - maximum
void Powell::count(int extremum)
{
  double m = 2.0 * extremum - 1.0;
  // step 1
  double **x = _points;
  int k;
  double d[3][2] = {{0, 1} , {1, 0} , {0, 1}};
  double *dNew[3];
  dNew[0] = new double [2];
  dNew[1] = new double [2];
  dNew[2] = new double [2];
  double y[4][2];
  y[0][0] = x[0][0];
  y[0][1] = x[0][1];
//  cout << fixed << setprecision(4);
  for(k = 0; k < POWELL_MAX - 1; k++)
  {
    // step 2 and 3
    _gold(y[1], d[0], y[0], m);
    _gold(y[2], d[1], y[1], m);
    if(y[2][0] == y[0][0] && y[2][1] == y[0][1])
    {
      x[k + 1][0] = y[2][0];
      x[k + 1][1] = y[2][1];
      break;
    }
    _gold(y[3], d[2], y[2], m);
    x[k + 1][0] = y[3][0];
    x[k + 1][1] = y[3][1];
    if(y[3][0] == y[1][0] && y[3][1] == y[1][1])
    {
      break;
    }
    // step 4
    if(sqrt((x[k + 1][0] - x[k][0]) * (x[k + 1][0] - x[k][0]) +
            (x[k + 1][1] - x[k][1]) * (x[k + 1][1] - x[k][1])) < _e)
    {
      break;
    }
    dNew[0][0] = y[3][0] - y[1][0];
    dNew[0][1] = y[3][1] - y[1][1];
    dNew[1][0] = d[2][0];
    dNew[1][1] = d[2][1];
    dNew[2][0] = dNew[0][0];
    dNew[2][1] = dNew[0][1];
    if(_rank(dNew) == 2)
    {
      for(int j = 0; j < 3; j++)
        for(int i = 0; i < 2; i++)
          d[j][i] = dNew[j][i];
    }
    y[0][0] = x[k + 1][0];
    y[0][1] = x[k + 1][1];
  }
  delete [] dNew[0];
  delete [] dNew[1];
  delete [] dNew[2];
  _nPoints = k + 1;
}

int Powell::_rank(double **d)
{
  double m[2];
  m[1] -= d[1][1] * d[1][0] / d[2][0];
  if(m[1] != 0)
    return 2;
  else
    return 1;
}

// minimum with other variable fixed
// gold section
void Powell::_gold(double *y, double *d, double *ys, double m)
{
  int i;
  double y1[2];
  double y2[2];
  double p1[2];
  double p2[2];
  double f1;
  double f2;
  if(abs(d[0]) > _e && abs(d[1]) > _e) // not 0
  {
    // cout << " diagonal\n";
    double k = d[1] / d[0];
    double b = ys[1] - k * ys[0];
    y1[0] = _calc->min(0);
    y1[1] = k * y1[0] + b;
    if(y1[1] < _calc->min(1))
    {
      y1[1] = _calc->min(1);
      y1[0] = (y1[1] - b) / k;
    }
    if(y1[1] > _calc->max(1))
    {
      y1[1] = _calc->max(1);
      y1[0] = (y1[1] - b) / k;
    }
    y2[0] = _calc->max(0);
    y2[1] = k * y2[0] + b;
    if(y2[1] < _calc->min(1))
    {
      y2[1] = _calc->min(1);
      y2[0] = (y2[1] - b) / k;
    }
    if(y2[1] > _calc->max(1))
    {
      y2[1] = _calc->max(1);
      y2[0] = (y2[1] - b) / k;
    }
    for(i = 0; i < POWELL_MAX; i++)
    {
      p1[0] = y1[0] + (3 - sqrt(5)) * (y2[0] - y1[0]) / 2;
      p1[1] = y1[1] + (3 - sqrt(5)) * (y2[1] - y1[1]) / 2;
      p2[0] = y1[0] + y2[0] - p1[0];
      p2[1] = y1[1] + y2[1] - p1[1];
      f1 = m * _calc->f(p1[0], p1[1]);
      f2 = m * _calc->f(p2[0], p2[1]);
      if(f1 < f2)
      {
        y2[0] = p2[0];
        y2[1] = p2[1];
      }
      else
      {
        y1[0] = p1[0];
        y1[1] = p1[1];
      }
      if(abs(p1[0] - p2[0]) < _e && abs(p1[1] - p2[1]) < _e)
        break;
    }
  }
  if(abs(d[0]) <= _e) // vetrical
  {
    // cout << " vertical\n";
    y1[0] = ys[0];
    y2[0] = ys[0];
    y1[1] = _calc->min(1);
    y2[1] = _calc->max(1);
    for(i = 0; i < POWELL_MAX; i++)
    {
      // cout << " gold " << i;
      p1[1] = y1[1] + (3 - sqrt(5)) * (y2[1] - y1[1]) / 2;
      p2[1] = y1[1] + y2[1] - p1[1];
      f1 = m * _calc->f(ys[0], p1[1]);
      f2 = m * _calc->f(ys[0], p2[1]);
      // cout << " p1[1] = " << setw(W) << p1[1]
      //      << "; p2[1] = " << setw(W) << p2[1]
      //      << "; f1 = " << setw(W) << f1
      //      << "; f2 = " << setw(W) << f2 << '\n';
      if(f1 < f2)
        y2[1] = p2[1];
      else
        y1[1] = p1[1];
      if(abs(p1[1] - p2[1]) < _e)
        break;
    }
  }
  if(abs(d[1]) <= _e) // horizontal
  {
    // cout << " horizontal\n";
    y1[0] = _calc->min(0);
    y2[0] = _calc->max(0);
    y1[1] = ys[1];
    y2[1] = ys[1];
    for(i = 0; i < POWELL_MAX; i++)
    {
      // cout << "  gold " << i;
      p1[0] = y1[0] + (3 - sqrt(5)) * (y2[0] - y1[0]) / 2;
      p2[0] = y1[0] + y2[0] - p1[0];
      f1 = m * _calc->f(p1[0], ys[1]);
      f2 = m * _calc->f(p2[0], ys[1]);
      // cout << " p1[0] = " << setw(W) << p1[0]
      //      << "; p2[0] = " << setw(W) << p2[0]
      //      << "; f1 = " << setw(W) << f1
      //      << "; f2 = " << setw(W) << f2 << '\n';
      if(f1 < f2)
        y2[0] = p2[0];
      else
        y1[0] = p1[0];
      if(abs(p1[0] - p2[0]) < _e)
        break;
    }
  }
  y[0] = (y1[0] + y2[0]) / 2;
  y[1] = (y1[1] + y2[1]) / 2;
}
