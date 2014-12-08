#ifndef CALC_H
#define CALC_H

#define MEMBERS 3
#define POINTS 101
#define MINPOINTS 11
#define MAXPOINTS 81
#define MINX -4.0
#define MAXX 4.0
#define MINY -4.0
#define MAXY 4.0

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
    double max(int);
    double min(int);
    int N();
    int N2();
    void countPlot();
    bool isCounted();

    void setN(int);
    void setMin(int, int);
    void setMax(int, int);

private:
    bool _is_counted;
    int _N; //points on axises
    double *_a; //coefficients
    double *_powx; //x powers
    double *_powy; //y powers
    /* displaying variables */
    double _max[3];
    double _min[3];
    double *_axis_x; //values on axis x
    double *_axis_y; //values on axis y
    double ** _z_plot; //values of function to plot

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
