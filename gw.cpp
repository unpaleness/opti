#include "calc.h"
#include "mw.h"
#include "gw.h"
#include "ui_gw.h"

GW::GW(QGLWidget *parent) : QGLWidget(parent), _gw(new Ui::GW)
{
  _initialized = false;
  for(int i = 0; i < 3; i++) _rot[i] = 0;
  _gw->setupUi(this);
}

GW::~GW()
{
  if(_initialized) //delete arrays only if they were initialized
    _eraseArrays();
  delete _gw;
}

void GW::initializeGL()
{
  qglClearColor(Qt::black);

  glEnable(GL_DEPTH_TEST);
//  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
//  glEnable(GL_LIGHTING);
//  glEnable(GL_LIGHT0);

//  static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
//  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GW::resizeGL(int w, int h)
{
  int side = qMin(w, h);
  glViewport((w-side)/2, (h-side)/2, side, side);
//  glViewport(0, 0, (GLint)w, (GLint)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  _windowWidth = w;
  _windowHeight = h;
//  #ifdef QT_OPENGL_ES_1
//    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
//  #else
//    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
//  #endif
//  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
//  glFrustum(_calc->minx(), _calc->maxx(), _calc->miny(), _calc->maxy(),
//          _calc->minz(), _calc->maxz());
  glOrtho(_calc->minx(), _calc->maxx(), _calc->miny(), _calc->maxy(),
          _calc->minz(), _calc->maxz());
  glMatrixMode(GL_MODELVIEW);
}

void GW::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity(); //загружаем матрицу
  glTranslated(0.0, 0.0, -100.0);
  glRotated(_rot[0] / 16.0, 1.0, 0.0, 0.0);
  glRotated(_rot[1] / 16.0, 0.0, 1.0, 0.0);
  glRotated(_rot[2] / 16.0, 0.0, 0.0, 1.0);
  _paintPoints();
  _paintAxises();
}

void GW::showEvent(QShowEvent *)
{
  if(!_initialized)
    _initializeArrays();
  _setPoints();
//  updateGL();
}

void GW::closeEvent(QCloseEvent *)
{
  _eraseArrays();
  _mw->setEnabled(true);
}

void GW::mousePressEvent(QMouseEvent *event)
{
  _lastPos = event->pos();
}

void GW::mouseMoveEvent(QMouseEvent *event)
{
  int dx = event->x() - _lastPos.x();
  int dy = event->y() - _lastPos.y();

  if(event->buttons() & Qt::LeftButton)
  {
    _setRotation(0, _rot[0] + 8 * dy);
    _setRotation(1, _rot[1] + 8 * dx);
  }
  else if(event->buttons() & Qt::RightButton)
  {
    _setRotation(0, _rot[0] + 8 * dy);
    _setRotation(2, _rot[2] + 8 * dx);
  }

    _lastPos = event->pos();
}

void GW::mainform(MW *mw) { _mw = mw; }

void GW::calcobject(Calc *calc) { _calc = calc; }

/*
 * PRIVATE
 */

template <class AnyClass>
void GW::_memalloc(int N, AnyClass *&A) { A = new AnyClass [N]; }

template <class AnyClass>
void GW::_memalloc(int N, int M, AnyClass **&A)
{
  A = new AnyClass *[N];
  for(int i = 0; i < N; i++)
    A[i] = new AnyClass [M];
}

template <class AnyClass>
void GW::_memfree(AnyClass *&A) { delete [] A; }
template <class AnyClass>
void GW::_memfree(int N, AnyClass **&A)
{
  for(int i = 0; i < N; i++)
    delete [] A[i];
  delete [] A;
}

void GW::_initializeArrays()
{
  _memalloc(_calc->N2(), 3, _pg);
  _initialized = true;
}

void GW::_eraseArrays()
{
  _memfree(_calc->N2(), _pg);
  _initialized = false;
}

void GW::_setPoints()
{
  //setting up points of axises
  for(int j = 0; j < 6; j++)
    for(int i = 0; i < 3; i++)
      _pa[j][i] = 0;
  _pa[0][0] = _calc->minx();
  _pa[1][0] = _calc->maxx();
  _pa[2][1] = _calc->miny();
  _pa[3][1] = _calc->maxy();
  _pa[4][2] = _calc->minz();
  _pa[5][2] = _calc->maxz();
  //setting up points of graphic
  int k = 0; //total counter
  for(int j = 0; j < _calc->N(); j++)
    for(int i = 0; i < _calc->N(); i++)
    {
      _pg[k][0] = _calc->x()[i];
      _pg[k][1] = _calc->y()[j];
      _pg[k][2] = _calc->z()[j][i];
      k++;
    }
}

void GW::_setRotation(int axis, int angle)
{
//  qNormalizeAngle(angle); //make angle never more than 360 degrees
  if(angle != _rot[axis])
  {
    _rot[axis] = angle;
    updateGL();
  }
}

void GW::_paintAxises()
{
  qglColor(Qt::white);
  for(int i = 0; i < 6; i += 2)
  {
    glBegin(GL_POINTS);
    glVertex3dv(_pa[i]);
    glVertex3dv(_pa[i + 1]);
    glEnd();
  }
}

void GW::_paintPoints()
{
  qglColor(Qt::red);
  glBegin(GL_POINTS);
  for(int i = 0; i < _calc->N2(); i++)
    glVertex3dv(_pg[i]);
  glEnd();
}
