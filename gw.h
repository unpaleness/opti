#ifndef GW_H
#define GW_H

#include <QPoint>
#include <QMouseEvent>
#include <QGLWidget>
//#include <QtOpenGL>

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

  void initializeGL(); //OpenGL initializing
  void resizeGL(int, int); //on window resize
  void paintGL(); //on unpdate
  void showEvent(QShowEvent *); //on window show
  void closeEvent(QCloseEvent *); //on window close
  void mousePressEvent(QMouseEvent *event); //on pressing mouse button
  void mouseMoveEvent(QMouseEvent *event); //on moving mouse

  void mainform(MW *); //main form connecting
  void calcobject(Calc *); //calc object connecting

private:
  Ui::GW *_gw; //graphic form (this)
  MW *_mw; //main form
  Calc *_calc; //calculating object

  bool
    _initialized; //flag that show that all arrays are initialized
  int
    _rot[3], //x,y,z-axis rotation angle
    _windowWidth, //window width
    _windowHeight; //window height
  QPoint _lastPos;
  GLdouble
    **_pg, //points of graphic
    _pa[6][3]; //points of axises

  template <class AnyClass>
  void _memalloc(int, AnyClass *&); //allocates memory to some array
  template <class AnyClass>
  void _memalloc(int, int, AnyClass **&); //allocates memory to some matrix
  template <class AnyClass>
  void _memfree(AnyClass *&); //frees memory from some array
  template <class AnyClass>
  void _memfree(int, AnyClass **&); //frees memory from some matrix

  void _initializeArrays(); //initializes all pointers
  void _eraseArrays(); //deletes all pointers
  void _setPoints(); //setspoints
  void _paintAxises(); //paints axises
  void _paintPoints(); //paints points
  void _setRotation(int, int); //set rotation angle
};

#endif // GW_H
