#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <cmath>

#define MAXSIMPLEXES 100

class Calc;

class Simplex
{
public:
  Simplex();
  ~Simplex();

  void setCalc(Calc *); //setting Calc-object
  void countSimpex(int); //implements counting process
  int nSimplex(); //returns amount of simplixes during solvation
  double ***simplex(); //returns simplex point

private:
  Calc *_calc; //calculating object

  int _nSimplex; //amount of simplexes
  double ***_simplex;
  double _alpha;
  double _beta;
  double _gamma;
  double _e;

  void _countSimplex(int); //optimizes using simplex-method
  template <class AnyClass>
  void _memalloc(int, int, int, AnyClass ***&);
  template <class AnyClass>
  void _memerase(int, int, AnyClass ***&);
  template <class T>
  void _sort(int, T *, bool); //sorting due to bool option
};

#endif // SIMPLEX_H
