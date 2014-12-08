#ifndef GW_H
#define GW_H

#include <QPoint>
#include <QMouseEvent>
#include <QGLWidget>
//#include <QtOpenGL>

#define ROLL_STEP 5
#define MIN_EYE_DISTANCE_X -400
#define MAX_EYE_DISTANCE_X 400
#define MIN_EYE_DISTANCE_Y -400
#define MAX_EYE_DISTANCE_Y 400
#define MIN_EYE_DISTANCE_Z -400
#define MAX_EYE_DISTANCE_Z 400

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
  void wheelEvent(QWheelEvent *); // on wheel roll
  void keyPressEvent(QKeyEvent *); //on pressing keyboard

  void mainform(MW *); //main form connecting
  void calcobject(Calc *); //calc object connecting

private:
  Ui::GW *_gw; //graphic form (this)
  MW *_mw; //main form
  Calc *_calc; //calculating object

  bool _isInitialized; //flag that show that all arrays are initialized
  bool _isNormilizedZ; //flag showing z-normalizing
  int _graphView; //view of graph: 0 - points, 1 - lines, 2 - faces
  int _rot[3]; //x,y,z-axis rotation angle
  int _windowWidth; //window width
  int _windowHeight; //window height
  QPoint _lastPos;
  GLdouble **_pg; //points of graphic
  GLdouble _pa[6][3]; //points of axises
  GLdouble _eyePosition[3]; //eye position o_O
  GLdouble **_normales; //normales to faces
  GLdouble _coefZ; //normalizing coefficient by z-axis

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
  void _setRotation(int, int); //set rotation angle
  void _paintAxises(); //paints axises
  void _paintPoints(); //paints points
  void _paintLines(); //paints lines
  void _paintFaces(); //paints faces
  void _countNormale(GLdouble *, GLdouble *, GLdouble *, GLdouble *,
                     GLdouble *);
};

#endif // GW_H
