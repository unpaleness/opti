#include "method.h"
#include "calc.h"

Method::Method()
{
  _e = 0.0001;
}

Method::~Method() {}

void Method::setCalc(Calc *calc) { _calc = calc; }

double *Method::extremum() { return _extremum; }

void Method::init(double *) {}

void Method::count(int) {}
