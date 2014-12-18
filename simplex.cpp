#include "simplex.h"
#include "calc.h"
#include <iostream>

Simplex::Simplex()
{
  _memalloc(SIMPLEX_MAX, 3, 3, _simplex);
  _alpha = 1.0;
  _beta = 0.5;
  _gamma = 2.0;
}

Simplex::~Simplex()
{
  _memerase(SIMPLEX_MAX, 3, _simplex);
}

int Simplex::nSimplex() { return _nSimplex; }

double ***Simplex::simplex() { return _simplex; }

/*
 * PRIVATE
 */

void Simplex::init(double *params)
{
  _simplex[0][0][0] = params[0];
  _simplex[0][0][1] = params[1];
  _simplex[0][0][2] = _calc->f(_simplex[0][0][0], _simplex[0][0][1]);
  _simplex[0][1][0] = params[2];
  _simplex[0][1][1] = params[3];
  _simplex[0][1][2] = _calc->f(_simplex[0][1][0], _simplex[0][1][1]);
  _simplex[0][2][0] = params[4];
  _simplex[0][2][1] = params[5];
  _simplex[0][2][2] = _calc->f(_simplex[0][2][0], _simplex[0][2][1]);
  _alpha = params[6];
  _beta = params[7];
  _gamma = params[8];
}

//0 - minimum, 1 - maximum
void Simplex::count(int extremum)
{
  //local variables

  //local variables
  bool isExtremumFound;
  double *ph;
  double *pg;
  double *pl;
  double *temp;
  double pc[3];
  double pr[3];
  double pe[3];
  double ps[3];
  //initial simplex
  _nSimplex = 0;
  _simplex[0][0][0] = _calc->x()[9];
  _simplex[0][0][1] = _calc->y()[10];
  _simplex[0][0][2] = _calc->z()[10][9];
  _simplex[0][1][0] = _calc->x()[10];
  _simplex[0][1][1] = _calc->y()[11];
  _simplex[0][1][2] = _calc->z()[11][10];
  _simplex[0][2][0] = _calc->x()[12];
  _simplex[0][2][1] = _calc->y()[8];
  _simplex[0][2][2] = _calc->z()[8][12];
  std::cout << "1 simplex\n";
  for(int i = 0; i < 3; i++)
    std::cout << 'p' << i + 1 << '(' << _simplex[0][i][0] << ';'
              << _simplex[0][i][1]  << ';' << _simplex[0][i][2] << ")\n";
  //Get... Set... COUNT!
  for(int s = 1; s < SIMPLEX_MAX; s++)
  {
    isExtremumFound = true;
    //preparing
    for(int j = 0; j < 3; j++)
      for(int i = 0; i < 3; i++)
        _simplex[s][j][i] = _simplex[s - 1][j][i];
    //sorting
    temp = _simplex[s][0];
    for(int i = 1; i < 3; i++)
      if(temp[2] < _simplex[s][i][2])
        temp = _simplex[s][i];
    ph = temp;
    temp = _simplex[s][0];
    for(int i = 1; i < 3; i++)
      if(temp[2] < _simplex[s][i][2] && _simplex[s][i][2] < ph[2])
        temp = _simplex[s][i];
    pg = temp;
    temp = _simplex[s][0];
    for(int i = 1; i < 3; i++)
      if(temp[2] > _simplex[s][i][2])
        temp = _simplex[s][i];
    pl = temp;
    //center of gravity
    pc[0] = (pg[0] + pl[0]) / 2.0;
    pc[1] = (pg[1] + pl[1]) / 2.0;
    //reflection
    pr[0] = (1.0 + _alpha) * pc[0] - _alpha * ph[0];
    pr[1] = (1.0 + _alpha) * pc[1] - _alpha * ph[1];
    pr[2] = _calc->z(pr[0], pr[1]);
    //conditions
    if(pr[2] < pl[2]) //successful direction
    {
      pe[0] = (1.0 - _gamma) * pc[0] + _gamma * pr[0];
      pe[1] = (1.0 - _gamma) * pc[1] + _gamma * pr[1];
      pe[2] = _calc->z(pe[0], pe[1]);
      if(pe[2] < pr[2])
        for(int i = 0; i < 3; i++)
          ph[i] = pe[i];
      else
        for(int i = 0; i < 3; i++)
          ph[i] = pr[i];
    }
    else if(pl[2] <= pr[2] && pr[2] < pg[2])
      for(int i = 0; i < 3; i++)
        ph[i] = pr[i];
    else
    {
      if(pg[2] <= pr[2] && pr[2] < ph[2])
      for(int i = 0; i < 3; i++)
      {
        pr[i] = pr[i] - ph[i];
        ph[i] += pr[i];
        pr[i] = ph[i] - pr[i];
      }
      //compression
      ps[0] = _beta * ph[0] + (1.0 - _beta) * pc[0];
      ps[1] = _beta * ph[1] + (1.0 - _beta) * pc[1];
      ps[2] = _calc->z(ps[0], ps[1]);
      if(ps[2] < ph[2])
        for(int i = 0; i < 3; i++)
          ph[i] = ps[i];
      else
      {
        for(int i = 0; i < 3; i++)
          ph[i] = pl[i] + (ph[i] - pl[i]) / 2.0;
        for(int i = 0; i < 3; i++)
          pg[i] = pl[i] + (pg[i] - pl[i]) / 2.0;
      }
    }
    //checking
    for(int i = 0; i < 3; i++)
    {
      int i2 = i + 1;
      if(i == 2)
        i2 = 0;
      if(sqrt((_simplex[s][i2][0] - _simplex[s][i][0]) *
              (_simplex[s][i2][0] - _simplex[s][i][0])) > _e)
        isExtremumFound = false;
    }
    //number of simplexes
    _nSimplex = s;
    std::cout << s + 1 << " simplex\n";
    for(int i = 0; i < 3; i++)
      std::cout << 'p' << i + 1 << '(' << _simplex[s][i][0] << ';'
                << _simplex[s][i][1]  << ';' << _simplex[s][i][2] << ")\n";
    if(isExtremumFound)
      break;
  }
}

template <class AnyClass>
void Simplex::_memalloc(int dim1, int dim2, int dim3, AnyClass ***&mat)
{
  mat = new AnyClass **[dim1];
  for(int j = 0; j < dim1; j++)
  {
    mat[j] = new AnyClass *[dim2];
    for(int i = 0; i < dim2; i++)
      mat[j][i] = new AnyClass [dim3];
  }
}

template <class AnyClass>
void Simplex::_memerase(int dim1, int dim2, AnyClass ***&arr)
{
  for(int j = 0; j < dim1; j++)
  {
    for(int i = 0; i < dim2; i++)
      delete [] arr[j][i];
    delete [] arr[j];
  }
  delete [] arr;
}

template <class T>
void _sort(int size, T *array, bool cond(T, T))
{
  T temp;
  for(int j = 0; j < size; j++)
    for(int i = j + 1; i < size; i++)
      if(!cond(array[i], array[j]))
      {
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
      }
}
