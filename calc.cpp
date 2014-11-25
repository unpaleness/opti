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
  _is_counted = false;
  _max_x = MAXX;
  _min_x = MINX;
  _max_y = MAXY;
  _min_y = MINY;
  _max_z = 0;
  _min_z = 0;
}

Calc::~Calc()
{
  _memerase(_a);
  _memerase(_powx);
  _memerase(_powy);
  _eraseinitialization();
}

double &Calc::a(int index) { return _a[index]; }
double &Calc::powx(int index) { return _powx[index]; }
double &Calc::powy(int index) { return _powy[index]; }
double *Calc::x() { return _axis_x; }
double *Calc::y() { return _axis_y; }
double **Calc::z() { return _z_plot; }
double Calc::maxx() { return _max_x; }
double Calc::minx() { return _min_x; }
double Calc::maxy() { return _max_y; }
double Calc::miny() { return _min_y; }
double Calc::maxz() { return _max_z; }
double Calc::minz() { return _min_z; }
int Calc::N() { return _N; }

void Calc::setN(int newN)
{
  _eraseinitialization();
  _N = newN;
  _initialize();
  _countplot();
}

void Calc::setMaxX(int maxx) { _max_x = maxx; }
void Calc::setMinX(int minx) { _min_x = minx; }
void Calc::setMaxY(int maxy) { _max_y = maxy; }
void Calc::setMinY(int miny) { _min_y = miny; }

void Calc::countPlot()
{
  _countplot();
  _is_counted = true;
}

bool Calc::isCounted() { return _is_counted; }

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

void Calc::_countplot()
{
  double
    dx = (_max_x - _min_x) / (_N - 1),
    dy = (_max_x - _min_y) / (_N - 1);
  _axis_x[0] = _min_x;
  _axis_y[0] = _min_y;
  for(int i = 1; i < _N; i++)
  {
    _axis_x[i] = _axis_x[i-1] + dx;
    _axis_y[i] = _axis_y[i-1] + dy;
  }
  for(int j = 0; j < _N; j++)
    for(int i = 0; i < _N; i++)
    {
      _z_plot[j][i] = _fun(_axis_y[j], _axis_x[i]);
      if(_z_plot[j][i] > _max_z) _max_z = _z_plot[j][i];
      if(_z_plot[j][i] < _min_z) _min_z = _z_plot[j][i];
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
    mat[i] = new AnyClass[dim2];
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
