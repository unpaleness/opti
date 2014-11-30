#ifndef GW_H
#define GW_H

#include <QGLWidget>

namespace Ui {
  class GW;
}

class MW;
class Calc;

class GW : public QGLWidget
{
  Q_OBJECT

public:
  explicit GW(QGLWidget *parent = 0);
  ~GW();

  void mainform(MW *);
  void calcobject(Calc *);

private:
  Ui::GW *_gw;
  MW *_mw;
  Calc *_calc;
};

#endif // GW_H
