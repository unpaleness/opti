#include <cmath>

#include "method.h"
#include "calc.h"

Method::Method()
{
  _isInitialized = false;
  _maxPoints = 0;
}
Method::~Method()
{
  if(_isInitialized)
    _memFree();
}

void Method::setCalc(Calc *calc) { _calc = calc; }
double *Method::extremum() { return _extremum; }
double Method::nPoints() { return _nPoints; }
double Method::points(int i, int coord) { return _points[i][coord]; }
void Method::setAccuracy(double e) { _e = e; }

void Method::memAlloc(int size)
{
  if(size > 0)
    _memRealloc(size);
}

void Method::init(double *) {}
void Method::count(int) {}

void Method::_memRealloc(int size)
{
  if(size != _maxPoints)
  {
    if(_isInitialized)
      _memFree();
    _maxPoints = size;
    _memAlloc();
    _isInitialized = true;
  }
}

void Method::_memAlloc()
{
  _points = new double *[_maxPoints];
  for(int i = 0; i < _maxPoints; i++)
    _points[i] = new double [2];
}

void Method::_memFree()
{
  for(int i = 0; i < _maxPoints; i++)
    delete [] _points[i];
  delete [] _points;
}

// gold section
void Method::_gold(double *y, double *d, double *ys, double m)
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

