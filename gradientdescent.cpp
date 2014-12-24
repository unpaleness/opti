#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#include "gradientdescent.h"
#include "calc.h"

GradientDescent::GradientDescent()
{

}

GradientDescent::~GradientDescent()
{

}

void GradientDescent::init(double *params)
{
  _points[0][0] = params[0];
  _points[0][1] = params[1];
}

void GradientDescent::count(int extremum)
{
  int k;
  double **x = _points;
  double m = 2.0 * extremum - 1.0;
  double e1 = _e;
  double e2 = _e;
  double **grad = new double *[_maxPoints];
  for(int i = 0; i < _maxPoints; i++)
    grad[i] = new double [2];
  ofstream output("gradient_descent.log", ios::out);
  output << fixed << setprecision(4);
  output << "\n x[0] = (" << x[0][0] << ';' << x[0][1] << ')';
  // step 2
  for(k = 0;; k++)
  {
    output << "\nk = " << k;
    // step 3
    grad[k][0] = m * _calc->dfdx(x[k][0], x[k][1]);
    grad[k][1] = m * _calc->dfdy(x[k][0], x[k][1]);
    output << "\n grad[" << k << "] = (" << grad[k][0] << ';' << grad[k][1]
           << ')';
    // step 4 and 5
    if(sqrt(grad[k][0] * grad[k][0] + grad[k][1] * grad[k][1]) < e1
       || k >= _maxPoints - 1)
    {
      output << "\n going to finish searching: ||grad(f(x[k]))|| < e1 "
             << "or k >= M";
      _extremum[0] = x[k][0];
      _extremum[1] = x[k][1];
      break;
    }
    // step 6 and 7
    _gold(x[k + 1], grad[k], x[k], m);
    output << "\n x[" << k + 1 << "] = (" << x[k + 1][0] << ';' << x[k + 1][1]
           << ')';
    // step 8
    if(sqrt((x[k + 1][0] - x[k][0]) * (x[k + 1][0] - x[k][0])
            + (x[k + 1][1] - x[k][1]) * (x[k + 1][1] - x[k][1])) < e2
            && fabs(m * _calc->f(x[k + 1][0], x[k + 1][1])
                    - m * _calc->f(x[k][0], x[k][1])) < e2)
    {
      output << "\n going to finish searching: ||x[k+1] < x[k]|| < e2 "
             << "and |f(x[k+1]) - f(x[k])| < e2";
      _extremum[0] = x[k + 1][0];
      _extremum[1] = x[k + 1][1];
      break;
    }
  }
  _nPoints = k + 2;
  output << "\nExtremum found at (" << _extremum[0] << ';' << _extremum[1]
       << "): " << m * _calc->f(_extremum[0], _extremum[1]);
  output.close();
  for(int i = 0; i < _maxPoints; i++)
    delete [] grad[i];
  delete [] grad;
}
