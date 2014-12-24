#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#include "powell.h"
#include "calc.h"

Powell::Powell()
{
}

Powell::~Powell()
{
}

void Powell::init(double *params)
{
  _points[0][0] = params[0];
  _points[0][1] = params[1];
}

// 1 - minimum, 0 - maximum
void Powell::count(int extremum)
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
  for(k = 0; k < _maxPoints - 1; k++)
  {
    output << "k = " << k << '\n';
    output << " directions:\n";
    output << ' ' << setw(W) << d[0][0] << setw(W) << d[1][0] << setw(W) << d[2][0] << '\n';
    output << ' ' << setw(W) << d[0][1] << setw(W) << d[1][1] << setw(W) << d[2][1] << '\n';
    output << " y0 = " << setw(W) << y[0][0] << ';' << setw(W) << y[0][1] << '\n';
    // step 2 and 3
    _gold(y[1], d[0], y[0], m);
    output << " y1 = " << setw(W) << y[1][0] << ';' << setw(W) << y[1][1] << '\n';
    _gold(y[2], d[1], y[1], m);
    output << " y2 = " << setw(W) << y[2][0] << ';' << setw(W) << y[2][1] << '\n';
    if(abs(y[2][0] - y[0][0]) < _e && abs(y[2][1] - y[0][1]) < _e)
    {
      output << " going to finish searching: yn = y0\n";
      x[k + 1][0] = y[2][0];
      x[k + 1][1] = y[2][1];
      break;
    }
    _gold(y[3], d[2], y[2], m);
    output << " y3 = " << setw(W) << y[3][0] << ';' << setw(W) << y[3][1] << '\n';
    x[k + 1][0] = y[3][0];
    x[k + 1][1] = y[3][1];
    if(abs(y[3][0] - y[1][0]) < _e && abs(y[3][1] - y[1][1]) < _e)
    {
      output << " going to finish searching: yn+1 = y1\n";
      break;
    }
    // step 4
    if(sqrt((x[k + 1][0] - x[k][0]) * (x[k + 1][0] - x[k][0]) +
            (x[k + 1][1] - x[k][1]) * (x[k + 1][1] - x[k][1])) < _e)
    {
      output << " going to finish searching: ||xk+1 - xk|| < e\n";
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
     output << " directions change\n";
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
  _extremum[0] = x[k + 1][0];
  _extremum[1] = x[k + 1][1];
  output << "Minimum is at x (" << setw(W) << x[k + 1][0] << setw(W) << x[k + 1][1] << ")\n";
  _nPoints = k + 2;
  output.close();
}

int Powell::_rank(double **d)
{
  double m = d[2][1] - d[1][1] * d[1][0] / d[2][0];
  if(abs(m) > _e)
    return 2;
  else
    return 1;
}
