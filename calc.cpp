//#include "simplex.h"
#include "calc.h"

/*
 * PUBLIC
 */

Calc::Calc()
{
  _N = POINTS;
  _memalloc(MEMBERS, _a);
  _memalloc(MEMBERS, _powx);
  _memalloc(MEMBERS, _powy);
  _initialize();
  _isCounted = false;
  _max[0] = MAXX;
  _min[0] = MINX;
  _max[1] = MAXY;
  _min[1] = MINY;
  _max[2] = 0;
  _min[2] = 0;
  _simplex = new Simplex;
  _simplex->setCalc(this);
}

Calc::~Calc()
{
  _memerase(_a);
  _memerase(_powx);
  _memerase(_powy);
  _eraseinitialization();
  delete _simplex;
}

double &Calc::a(int index) { return _a[index]; }
double &Calc::powx(int index) { return _powx[index]; }
double &Calc::powy(int index) { return _powy[index]; }
double *Calc::x() { return _axis_x; }
double *Calc::y() { return _axis_y; }
double **Calc::z() { return _z_plot; }
double Calc::z(double x, double y) { return _fun(y, x); }
double Calc::max(int i) { return _max[i]; }
double Calc::min(int i) { return _min[i]; }
int Calc::N() { return _N; }
int Calc::N2() { return _N * _N; }
Simplex *Calc::simplex() { return _simplex; }
void Calc::setMax(int i, int max) { _max[i] = max; }
void Calc::setMin(int i, int min) { _min[i] = min; }

void Calc::countPlot()
{
  _countPlot();
  _isCounted = true;
}

//0 - Simplex (Nelder-Mid)
void Calc::optimize(int method)
{
  switch(method)
  {
    case 0:
      _simplex->countSimpex(_extremum);
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

double Calc::_fun(double y, double x)
{
    return _a[0] * pow(x, _powx[0]) * pow(y, _powy[0]) +
            _a[1] * pow(x, _powx[1]) * pow(y, _powy[1]) +
            _a[2] * pow(x, _powx[2]) * pow(y, _powy[2]);
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
  for(int j = 0; j < _N; j++)
    for(int i = 0; i < _N; i++)
    {
      _z_plot[j][i] = _fun(_axis_y[j], _axis_x[i]);
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
