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
  double nPoints();
  double points(int, int);
  void setAccuracy(double);
  void memAlloc(int);

  virtual void init(double *);
  virtual void count(int);

protected:
  Calc *_calc;

  double _e;
  double _extremum[2];
  double _maxPoints;
  double _nPoints;
  double **_points;

private:
  bool _isInitialized;

  void _memFree();
  void _memAlloc();
  void _memRealloc(int);

};

#endif // METHOD_H
