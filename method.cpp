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
