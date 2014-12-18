#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <cmath>

#include "method.h"

#define SIMPLEX_MAX 100

class Calc;

class Simplex : public Method
{
public:
  Simplex();
  ~Simplex();

  void init(double *); // three initial points, alpha, beta, gamma
  void count(int); //implements counting process
  int nSimplex(); //returns amount of simplixes during solvation
  double ***simplex(); //returns simplex point

private:
  int _nSimplex; //amount of simplexes
  double ***_simplex;
  double _alpha;
  double _beta;
  double _gamma;

  template <class AnyClass>
  void _memalloc(int, int, int, AnyClass ***&);
  template <class AnyClass>
  void _memerase(int, int, AnyClass ***&);
  template <class T>
  void _sort(int, T *, bool); //sorting due to bool option
};

#endif // SIMPLEX_H
