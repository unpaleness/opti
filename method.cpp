#include "method.h"
#include "calc.h"

Method::Method()
{
  _e = 0.001;
}

Method::~Method() {}

void Method::setCalc(Calc *calc) { _calc = calc; }

void Method::init(double *) {}

void Method::count(int) {}
