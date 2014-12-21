#ifndef METHOD_H
#define METHOD_H

class Calc;

class Method
{
public:
  Method();
  virtual ~Method();

  void setCalc(Calc *);
  double *extremum();

  virtual void init(double *);
  virtual void count(int);

protected:
  Calc *_calc;

  double _e;
  double _extremum[2];

};

#endif // METHOD_H
