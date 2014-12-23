#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#include "fletcherreeves.h"
#include "calc.h"

FletcherReeves::FletcherReeves()
{
}

FletcherReeves::~FletcherReeves()
{
}

void FletcherReeves::init(double *params)
{
  _points[0][0] = params[0];
  _points[0][1] = params[1];
}

// 1 - minimum, 0 - maximum
void FletcherReeves::count(int extremum)
{
  ofstream output("powell.log", ios::out);
  output << fixed << setprecision(4);
  output << "f(x;y) = ";
  for(int i = 0; i < 6; i++)
  {
    if(i > 0 && _calc->a(0, i) > 0)
      output << '+';
    output << _calc->a(0, i) << "*x^" << _calc->powx(0, i) << "*y^"
           << _calc->powy(0, i);
    output << '\n';
  }
  double m = 2.0 * extremum - 1.0;
  output << fixed << setprecision(4);
  int k;
  double _e1;
  double _e2;
  double **x;
  double **d;
  double **grad;
  double *beta;
  d = new double *[_maxPoints];
  grad = new double *[_maxPoints];
  beta = new double [_maxPoints];
  for(int i = 0; i < _maxPoints; i++)
  {
    d[i] = new double [2];
    grad[i] = new double [2];
  }
  // step 1
  _e1 = _e;
  _e2 = _e;
  x = _points;
  // step 2
  for(k = 0;; k++)
  {
    output << "k = " << k << '\n';
    // step 3
    grad[k][0] = m * _calc->dfdx(x[k][0], x[k][1]);
    grad[k][1] = m * _calc->dfdy(x[k][0], x[k][1]);
    output << " x[" << k << "] = (" << setw(W) << x[k][0] << ';'
         << setw(W) << x[k][1] << ")\n";
    output << " grad[" << k << "] = (" << setw(W) << grad[k][0] << ';'
         << setw(W) << grad[k][1] << ")\n";
    // step14
    if(sqrt(grad[k][0] * grad[k][0] + grad[k][1] * grad[k][1]) < _e1)
    {
      output << " going to finish searching: ||grad(f(xk))|| < e1\n";
      break;
    }
    // step 5
    if(k >= _maxPoints)
    {
      output << " going to finish searching: k >= M\n";
      break;
    }
    if(k == 0)
    {
      // step 6
      d[k][0] = grad[k][0];
      d[k][1] = grad[k][1];
    }
    else
    {
      // step 7
      beta[k - 1] = (grad[k][0] * grad[k][0] + grad[k][1] * grad[k][1]) /
                    (grad[k - 1][0] * grad[k - 1][0] +
                    grad[k - 1][1] * grad[k - 1][1]);
      output << " beta[" << k - 1 << "] = " << setw(W) << beta[k - 1]
           << '\n';
      // step 8
      d[k][0] = - grad[k][0] + beta[k - 1] * d[k - 1][0];
      d[k][1] = - grad[k][1] + beta[k - 1] * d[k - 1][1];
    }
    output << " d[" << k << "] = (" << setw(W) << d[k][0] << ';'
         << setw(W) << d[k][1] << ")\n";
    // step 9 and 10
    _gold(x[k + 1], d[k], x[k], m);
    // step 11
    if(sqrt((x[k + 1][0] - x[k][0]) * (x[k + 1][0] - x[k][0]) +
       (x[k + 1][1] - x[k][1]) * (x[k + 1][1] - x[k][1])) < _e2 &&
       abs(m * _calc->f(x[k + 1][0], x[k + 1][1]) - m * _calc->f(x[k][0], x[k][1])) < _e2)
    {
      output << " going to finish searching: ||x[k+1] - x[k]|| < e2"
           << " and |f(x[k+1]) - f(x[k])| < e2\n";
      break;
    }
  }
  _nPoints = k + 2;
  _extremum[0] = x[k + 1][0];
  _extremum[1] = x[k + 1][1];
  output << "Minimum found at (" << setw(W) << x[k + 1][0] << ';'
       << setw(W) << x[k + 1][1] << ")\n";
  output.close();
  for(int i = 0; i < _maxPoints; i++)
  {
    delete [] d[i];
    delete [] grad[i];
  }
  delete [] d;
  delete [] grad;
  delete [] beta;
}

// minimum with other variable fixed
// gold section
void FletcherReeves::_gold(double *y, double *d, double *ys, double m)
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
    for(i = 0; i < _maxPoints; i++)
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
    for(i = 0; i < _maxPoints; i++)
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
    for(i = 0; i < _maxPoints; i++)
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
