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
  _fletcherReeves = new FletcherReeves;
  _simplex->setCalc(this);
  _powell->setCalc(this);
  _fletcherReeves->setCalc(this);
}

Calc::~Calc()
{
  _memerase(CALC_FUNCTIONS, _a);
  _memerase(CALC_FUNCTIONS, _powx);
  _memerase(CALC_FUNCTIONS, _powy);
  _eraseinitialization();
  delete _simplex;
  delete _powell;
  delete _fletcherReeves;
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
void Calc::setMax(int i, double max) { _max[i] = max; }
void Calc::setMin(int i, double min) { _min[i] = min; }

Simplex *Calc::simplex() { return _simplex; }
Powell *Calc::powell() { return _powell; }
FletcherReeves *Calc::fletcherReeves() { return _fletcherReeves; }
double Calc::f(double x, double y) { return _fun(0, y, x); }
double Calc::dfdx(double x, double y) { return _fun(1, y, x); }
double Calc::dfdy(double x, double y) { return _fun(2, y, x); }

void Calc::initMethods(double e, int maxPoints)
{
  _simplex->setAccuracy(e);
  _fletcherReeves->setAccuracy(e);
  _powell->setAccuracy(e);
  _simplex->memAlloc(maxPoints);
  _fletcherReeves->memAlloc(maxPoints);
  _powell->memAlloc(maxPoints);
}

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
      _fletcherReeves->init(_lineEdits);
      _fletcherReeves->count(extremum);
      break;
    case 2:
      _simplex->init(_lineEdits);
      _simplex->count(extremum);
    default:
      break;
  }
}

double *Calc::getExtremum(int method)
{
  switch(method)
  {
    case 0:
      return _powell->extremum();
    case 1:
      return _fletcherReeves->extremum();
    case 2:
      return _simplex->extremum();
    default:
      break;
  }
  return nullptr;
}

int Calc::getNPoints(int method)
{
  switch(method)
  {
    case 0:
      return _powell->nPoints();
    case 1:
      return _fletcherReeves->nPoints();
    case 2:
      return _simplex->nPoints() / 3;
    default:
      break;
  }
  return 0;
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
  double res = 0;
  for(int i = 0; i < CALC_MEMBERS; i++)
    res += _a[fu][i] * pow(x, _powx[fu][i]) * pow(y, _powy[fu][i]);
  return res;
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
