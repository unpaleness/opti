#ifndef CALC_H
#define CALC_H

#include <cmath>

#include "simplex.h"

#define CALC_FUNCTIONS 6
#define CALC_MEMBERS 3

//class Simplex;

class Calc
{
public:
    Calc();
    ~Calc();

    double &a(int, int);
    double &powx(int, int);
    double &powy(int, int);
    double *x();
    double *y();
    double **z();
    double z(double, double);
    double max(int);
    double min(int);
    double step(int);
    int N();
    int N2();
    void countPlot(); //count plot for illustration
    void optimize(int); //optimize with seleceted method
    bool isCounted();
    Simplex *simplex();
    void setMin(int, int);
    void setMax(int, int);

private:
    Simplex *_simplex; //simplex-method object

    bool _isCounted;
    int _extremum; //type of extermum
    int _N; //points on axises
    double **_a; //coefficients
    double **_powx; //x powers
    double **_powy; //y powers
    /* displaying variables */
    double _max[3];
    double _min[3];
    double *_axis_x; //values on axis x
    double *_axis_y; //values on axis y
    double ** _z_plot; //values of function to plot

    void _initialize(); //initialize plotting arrays
    void _eraseinitialization(); //erase memory after plotting arrays
    double _fun(int, double, double);
    void _countPlot();
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
