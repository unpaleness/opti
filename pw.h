#ifndef PW_H
#define PW_H

#include <QGLWidget>

namespace Ui {
  class PW;
}

class Calc;
class MW;
class GW;

class PW : public QGLWidget
{
  Q_OBJECT

public:
  explicit PW(QGLWidget *parent = 0);
  ~PW();

  void showEvent(QShowEvent *);
  void closeEvent(QCloseEvent *);
  void initializeGL(); //OpenGL initializing
  void resizeGL(int, int); //on window resize
  void paintGL(); //on unpdate

  void setCalc(Calc *);
  void setMainForm(MW *);
  void setGraphicForm(GW *);

private:
  Ui::PW *_pw;
  Calc *_calc;
  MW *_mw;
  GW *_gw;

  int _gridLinesX;
  int _gridLinesY;

  bool _isInitialized; //flag that show that all arrays are initialized
  GLdouble **_pg; //points of graphic

  void _setPoints();
  void _paintPoints();
  void _paintAxises();
  void _paintWay();
};

#endif // PW_H
