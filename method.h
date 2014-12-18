#ifndef METHOD_H
#define METHOD_H

class Calc;

class Method
{
public:
  Method();
  virtual ~Method();

  void setCalc(Calc *);

  virtual void init(double *);
  virtual void count(int);

protected:
  Calc *_calc;

  double _e;

};

#endif // METHOD_H
