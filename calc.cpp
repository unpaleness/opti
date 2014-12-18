//#include "simplex.h"
#include "calc.h"

/*
 * PUBLIC
 */

Calc::Calc()
{
  _N = 101;
  _memalloc(CALC_FUNCTIONS, CALC_MEMBERS, _a);
  _memalloc(CALC_FUNCTIONS, CALC_MEMBERS, _powx);
  _memalloc(CALC_FUNCTIONS, CALC_MEMBERS, _powy);
  _initialize();
  _isCounted = false;
  _max[0] = 10.0;
  _min[0] = -10.0;
  _max[1] = 10.0;
  _min[1] = -10.0;
  _max[2] = 0;
  _min[2] = 0;
  _simplex = new Simplex;
  _powell = new Powell;
  _simplex->setCalc(this);
  _powell->setCalc(this);
}

Calc::~Calc()
{
  _memerase(CALC_FUNCTIONS, _a);
  _memerase(CALC_FUNCTIONS, _powx);
  _memerase(CALC_FUNCTIONS, _powy);
  _eraseinitialization();
  delete _simplex;
  delete _powell;
}

double &Calc::a(int fu, int index) { return _a[fu][index]; }
double &Calc::powx(int fu, int index) { return _powx[fu][index]; }
double &Calc::powy(int fu, int index) { return _powy[fu][index]; }
double &Calc::lineEdits(int index) { return _lineEdits[index]; }
double *Calc::x() { return _axis_x; }
double *Calc::y() { return _axis_y; }
double **Calc::z() { return _z_plot; }
double Calc::z(double x, double y) { return _fun(0, y, x); }
double Calc::max(int i) { return _max[i]; }
double Calc::min(int i) { return _min[i]; }
double Calc::step(int i) { return (_max[i] - _min[i]) / (_N - 1); }
int Calc::N() { return _N; }
int Calc::N2() { return _N * _N; }
void Calc::setMax(int i, int max) { _max[i] = max; }
void Calc::setMin(int i, int min) { _min[i] = min; }

Simplex *Calc::simplex() { return _simplex; }
Powell *Calc::powell() { return _powell; }
double Calc::f(double x, double y) { return _fun(0, y, x); }
double Calc::dfdx(double x, double y) { return _fun(1, y, x); }
double Calc::dfdy(double x, double y) { return _fun(2, y, x); }

void Calc::countPlot()
{
  _countPlot();
  _isCounted = true;
}

//0 - Simplex (Nelder-Mid)
void Calc::optimize(int method, int extremum)
{
  switch(method)
  {
    case 0:
      _powell->init(_lineEdits);
      _powell->count(extremum);
      break;
    case 1:
      _simplex->init(_lineEdits);
      _simplex->count(extremum);
      break;
    default:
      break;
  }
}

bool Calc::isCounted() { return _isCounted; }

/*
 * PRIVATE
 */

void Calc::_initialize()
{
  _memalloc(_N, _axis_x);
  _memalloc(_N, _axis_y);
  _memalloc(_N, _N, _z_plot);
}

void Calc::_eraseinitialization()
{
  _memerase(_axis_x);
  _memerase(_axis_y);
  _memerase(_N, _z_plot);
}

double Calc::_fun(int fu, double y, double x)
{
    return _a[fu][0] * pow(x, _powx[fu][0]) * pow(y, _powy[fu][0]) +
            _a[fu][1] * pow(x, _powx[fu][1]) * pow(y, _powy[fu][1]) +
            _a[fu][2] * pow(x, _powx[fu][2]) * pow(y, _powy[fu][2]);
}

void Calc::_countPlot()
{
  double
    dx = (_max[0] - _min[0]) / (_N - 1),
    dy = (_max[1] - _min[1]) / (_N - 1);
  _axis_x[0] = _min[0];
  _axis_y[0] = _min[1];
  for(int i = 1; i < _N; i++)
  {
    _axis_x[i] = _axis_x[i-1] + dx;
    _axis_y[i] = _axis_y[i-1] + dy;
  }
  _max[2] = _fun(0, _axis_y[_N / 2], _axis_x[_N / 2]);
  _min[2] = _fun(0, _axis_y[_N / 2], _axis_x[_N / 2]);
  for(int j = 0; j < _N; j++)
    for(int i = 0; i < _N; i++)
    {
      _z_plot[j][i] = _fun(0, _axis_y[j], _axis_x[i]);
      if(_z_plot[j][i] > _max[2]) _max[2] = _z_plot[j][i];
      if(_z_plot[j][i] < _min[2]) _min[2] = _z_plot[j][i];
    }
}

template <class AnyClass>
void Calc::_memalloc(int dim1, AnyClass *&arr)
{
  arr = new AnyClass [dim1];
}

template <class AnyClass>
void Calc::_memalloc(int dim1, int dim2, AnyClass **&mat)
{
  mat = new AnyClass *[dim1];
  for(int i = 0; i < dim1; i++)
    mat[i] = new AnyClass [dim2];
}

template <class AnyClass>
void Calc::_memerase(AnyClass *&arr)
{
  delete [] arr;
}

template <class AnyClass>
void Calc::_memerase(int dim1, AnyClass **&arr)
{
  for(int i = 0; i < dim1; i++)
    delete [] arr[i];
  delete [] arr;
}
