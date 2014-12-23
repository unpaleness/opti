#include "simplex.h"
#include "calc.h"
#include <iostream>
#include <fstream>

using namespace std;

Simplex::Simplex()
{
  _alpha = 1.0;
  _beta = 0.5;
  _gamma = 2.0;
}

Simplex::~Simplex() {}

/*
 * PRIVATE
 */

void Simplex::init(double *params)
{
  _points[0][0] = params[0];
  _points[0][1] = params[1];
  _points[1][0] = params[2];
  _points[1][1] = params[3];
  _points[2][0] = params[4];
  _points[2][1] = params[5];
  _alpha = params[6];
  _beta = params[7];
  _gamma = params[8];
}

//1 - minimum, 0 - maximum
void Simplex::count(int extremum)
{
  double m = 2.0 * extremum - 1.0;
  ofstream output("simplex.log", ios::out);
  // step 1
  int k;
  double **x = _points;
  double sigma;
  double fx[7];
  double xt[7][2];
  for(int i = 0; i < 3; i++)
    output << "\nx[" << i << "] = (" << x[i][0] << ';' << x[i][1] << ")";
  for(k = 0; k < _maxPoints - 6; k += 3)
  {
    output << "\nk = " << k;
    for(int i = 0; i < 3; i++)
    {
      xt[i][0] = x[k + i][0];
      xt[i][1] = x[k + i][1];
      fx[i] = m * _calc->f(xt[i][0], xt[i][1]);
    }
    // step 2
    for(int j = 0; j < 3; j++)
      for(int i = j; i < 3; i++)
        if(fx[i] < fx[j])
        {
          xt[j][0] -= xt[i][0];
          xt[i][0] += xt[j][0];
          xt[j][0] = xt[i][0] - xt[j][0];
          xt[j][1] -= xt[i][1];
          xt[i][1] += xt[j][1];
          xt[j][1] = xt[i][1] - xt[j][1];
          fx[j] -= fx[i];
          fx[i] += fx[j];
          fx[j] = fx[i] - fx[j];
        }
    for(int i = 0; i < 3; i++)
    output << "\n f[" << i << "] = f(" << xt[i][0] << ';' << xt[i][1] << ") = "
         << fx[i];
    // step 3
    // center
    xt[3][0] = (xt[0][0] + xt[1][0]) / 2;
    xt[3][1] = (xt[0][1] + xt[1][1]) / 2;
    fx[3] = m * _calc->f(xt[3][0], xt[3][1]);
    output << "\n f[3] = f(" << xt[3][0] << ';' << xt[3][1] << ") = " << fx[3];
    // step 4
    sigma = 0;
    for(int i = 0; i < 3; i++)
      sigma += (fx[i] - fx[3]) * (fx[i] - fx[3]);
    sigma = sqrt(sigma / 3);
    if(sigma <= _e)
    {
      output << "\n going to finish searching: sigma <= e";
      _extremum[0] = xt[0][0];
      _extremum[1] = xt[0][1];
      break;
    }
    // step 5
    // reflection
    xt[4][0] = xt[3][0] + _alpha * (xt[3][0] - xt[2][0]);
    xt[4][1] = xt[3][1] + _alpha * (xt[3][1] - xt[2][1]);
    fx[4] = m * _calc->f(xt[4][0], xt[4][1]);
    output << "\n f[4] = f(" << xt[4][0] << ';' << xt[4][1] << ") = " << fx[4];
    // step 6
    // a)
    if(fx[4] <= fx[0])
    {
      // extension
      xt[5][0] = xt[3][0] + _gamma * (xt[4][0]) - xt[3][0];
      xt[5][1] = xt[3][1] + _gamma * (xt[4][1]) - xt[3][1];
      fx[5] = m * _calc->f(xt[5][0], xt[5][1]);
      output << "\n f[5] = f(" << xt[5][0] << ';' << xt[5][1] << ") = "
             << fx[5];
      output << "\n f_reflection <= f_lowest";
      if(fx[5] < fx[0])
      {
        x[k + 3][0] = xt[0][0];
        x[k + 3][1] = xt[0][1];
        x[k + 4][0] = xt[1][0];
        x[k + 4][1] = xt[1][1];
        x[k + 5][0] = xt[5][0];
        x[k + 5][1] = xt[5][1];
        output << "\n  f_extension < f_lowest\n  making extension";
      }
      else
      {
        x[k + 3][0] = xt[0][0];
        x[k + 3][1] = xt[0][1];
        x[k + 4][0] = xt[1][0];
        x[k + 4][1] = xt[1][1];
        x[k + 5][0] = xt[4][0];
        x[k + 5][1] = xt[4][1];
        output << "\n  f_extension >= f_lowest\n  making reflection";
      }
    }
    // b)
    if(fx[1] < fx[4] && fx[4] <= fx[2])
    {
      // compression
      xt[6][0] = xt[3][0] + _beta * (xt[2][0] - xt[3][0]);
      xt[6][1] = xt[3][1] + _beta * (xt[2][1] - xt[3][1]);
      fx[6] = m * _calc->f(xt[6][0], xt[6][1]);
      output << "\n f[6] = f(" << xt[6][0] << ';' << xt[6][1] << ") = "
             << fx[6];
      x[k + 3][0] = xt[0][0];
      x[k + 3][1] = xt[0][1];
      x[k + 4][0] = xt[1][0];
      x[k + 4][1] = xt[1][1];
      x[k + 5][0] = xt[6][0];
      x[k + 5][1] = xt[6][1];
      output << "\n f_second < f_reflection <= f_highest\n making compression";
    }
    // c)
    if(fx[0] < fx[4] && fx[4] <= fx[1])
    {
      x[k + 3][0] = xt[0][0];
      x[k + 3][1] = xt[0][1];
      x[k + 4][0] = xt[1][0];
      x[k + 4][1] = xt[1][1];
      x[k + 5][0] = xt[4][0];
      x[k + 5][1] = xt[4][1];
      output << "\n f_lowest < f_reflection <= f_second\n making reflection";
    }
    // d)
    if(fx[4] > fx[2])
    {
      // reduction
      for(int i = 0; i < 3; i++)
      {
        x[k + 3 + i][0] = xt[0][0] + (xt[i][0] - xt[0][0]) / 2;
        x[k + 3 + i][1] = xt[0][1] + (xt[i][1] - xt[0][1]) / 2;
      }
      output << "\n f_reflection > f_highest\n making reduction";
    }
    for(int i = 0; i < 3; i++)
    output << " \nx[" << k + 3 + i << "] = (" << x[k + 3 + i][0] << ';'
         << x[k + 3 + i][1] << ")";
  }
  _nPoints = k + 6;
  output << "\nExtremum found at (" << _extremum[0] << ';' << _extremum[1]
       << "): " << m * _calc->f(_extremum[0], _extremum[1]);
  output.close();
}
