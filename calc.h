#ifndef CALC_H
#define CALC_H

#include <cmath>

#include "simplex.h"
#include "powell.h"
#include "fletcherreeves.h"
#include "gradientdescent.h"

#define CALC_FUNCTIONS 3
#define CALC_MEMBERS 6
#define CALC_METHODS 4

//class Simplex;

class Calc
{
public:
    Calc();
    ~Calc();

    double &a(int, int);
    double &powx(int, int);
    double &powy(int, int);
    double &lineEdits(int);
    double *x();
    double *y();
    double **z();
    double *xP();
    double *yP();
    double **zP();
    double z(double, double);
    double max(int);
    double min(int);
    double step(int);
    int N();
    int N2();
    int NPlanar();
    int N2Planar();
    void countPlot(); // count plot for illustration
    void optimize(int, int); // optimize with seleceted method by min or max
    double *getExtremum(int); // gets extremum with selected method
    int getNPoints(int); // gets amount of iterations with selected method
    bool isCounted();
    Method *method(int);
//    Simplex *simplex();
//    Powell *powell();
//    FletcherReeves *fletcherReeves();
    void initMethods(double, int);
    void setMin(int, double);
    void setMax(int, double);

    double f(double, double);
    double dfdx(double, double);
    double dfdy(double, double);

private:
    Method *_methods[CALC_METHODS];
//    Simplex *_simplex; //simplex-method object
//    Powell *_powell; //powell-method object
//    FletcherReeves *_fletcherReeves;

    bool _isCounted;
    int _N; //points on axises
    int _NPlanar; //point to planar graph
    double **_a; //coefficients
    double **_powx; //x powers
    double **_powy; //y powers
    double _lineEdits[10]; //values of optional line edits
    /* displaying variables */
    double _max[3];
    double _min[3];
    double *_axis_x; //values on axis x
    double *_axis_y; //values on axis y
    double ** _z_plot; //values of function to plot
    double *_xPlanar;
    double *_yPlanar;
    double **_zPlanar;

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
