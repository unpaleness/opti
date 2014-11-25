#ifndef CALC_H
#define CALC_H

#define MEMBERS 3
#define POINTS 41
#define MINPOINTS 11
#define MAXPOINTS 81
#define MINX -10.0
#define MAXX 10.0
#define MINY -10.0
#define MAXY 10.0

#include <cmath>

class Calc
{
public:
    Calc();
    ~Calc();

    double &a(int);
    double &powx(int);
    double &powy(int);
    double *x();
    double *y();
    double **z();
    double maxx();
    double minx();
    double maxy();
    double miny();
    double maxz();
    double minz();
    int N();
    void countPlot();
    bool isCounted();

    void setN(int);
    void setMinX(int);
    void setMaxX(int);
    void setMinY(int);
    void setMaxY(int);

private:
    bool
      _is_counted;
    int
      _N; //points on axises
    double
      *_a, //coefficients
      *_powx, //x powers
      *_powy, //y powers
      /* displaying variables */
      _max_x,
      _min_x,
      _max_y,
      _min_y,
      _max_z,
      _min_z,
      *_axis_x, //values on axis x
      *_axis_y, //values on axis y
      ** _z_plot; //values of function to plot

    void _initialize(); //initialize plotting arrays
    void _eraseinitialization(); //erase memory after plotting arrays
    double _fun(double, double);
    void _countplot();
    template <class AnyClass>
    void _memalloc(int, AnyClass *&);
    template <class AnyClass>
    void _memalloc(int, int, AnyClass **&);
    template <class AnyClass>
    void _memerase(AnyClass *&);
    template <class AnyClass>
    void _memerase(int, AnyClass **&);
};

#endif // CALC_H
