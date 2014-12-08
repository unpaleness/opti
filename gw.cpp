#include "calc.h"
#include "mw.h"
#include "gw.h"
#include "ui_gw.h"
#include <iostream>

GW::GW(QGLWidget *parent) : QGLWidget(parent), _gw(new Ui::GW)
{
  _isInitialized = false;
  _isNormilizedZ = false;
  _graphView = 0;
  _eyePosition[0] = 0;
  _eyePosition[1] = 0;
  _eyePosition[2] = 0;
  for(int i = 0; i < 3; i++) _rot[i] = 0;
  _gw->setupUi(this);
}

GW::~GW()
{
  if(_isInitialized) //delete arrays only if they were initialized
    _eraseArrays();
  delete _gw;
}

void GW::initializeGL()
{
  qglClearColor(Qt::black);

  glEnable(GL_DEPTH_TEST);
//  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  static GLfloat lightPosition[4] = { 2.0, 2.0, 1.0, 0.0 };
  static GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightDiffuse);
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
//  glOrtho(_calc->min(0) * 2.0, _calc->max(0) * 2.0, _calc->min(1) * 2.0,
//          _calc->max(1) * 2.0, _calc->min(2) * 2.0, _calc->max(2) * 2.0);
  glOrtho(_calc->min(0) * 2.0, _calc->max(0) * 2.0, _calc->min(1) * 2.0,
          _calc->max(1) * 2.0, -1000, 1000);
  glFrustum(-1.0, 1.0, -1.0, 1.0, 100.0, 50.0);
  glMatrixMode(GL_MODELVIEW);
}

void GW::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity(); //загружаем матрицу
  glTranslated(_eyePosition[0], _eyePosition[1], _eyePosition[2]);
  if(_isNormilizedZ)
    glScaled(1.0, 1.0, _coefZ);
  glRotated(_rot[0] / 16.0, 1.0, 0.0, 0.0);
  glRotated(_rot[1] / 16.0, 0.0, 1.0, 0.0);
  glRotated(_rot[2] / 16.0, 0.0, 0.0, 1.0);
  switch(_graphView)
  {
    case 1: _paintLines(); break;
    case 2: _paintFaces(); break;
    default: _paintPoints(); break;
  }
  _paintAxises();
}

void GW::showEvent(QShowEvent *)
{
  if(!_isInitialized)
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

void GW::wheelEvent(QWheelEvent *event)
{
  if(event->delta() > 0)
  {
    _eyePosition[2] += ROLL_STEP;
    if(_eyePosition[2] > MAX_EYE_DISTANCE_Z)
      _eyePosition[2] = MAX_EYE_DISTANCE_Z;
    updateGL();
  }
  if(event->delta() < 0)
  {
    _eyePosition[2] -= ROLL_STEP;
    if(_eyePosition[2] < MIN_EYE_DISTANCE_Z)
      _eyePosition[2] = MIN_EYE_DISTANCE_Z;
    updateGL();
  }
}

void GW::keyPressEvent(QKeyEvent *event)
{
  switch(event->key())
  {
    case Qt::Key_N:
      _isNormilizedZ = !_isNormilizedZ;
      updateGL();
      break;
    case Qt::Key_V:
      _graphView++;
      if(_graphView > 2)
        _graphView = 0;
      updateGL();
      break;
    default:
      break;
  }
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
  _memalloc((_calc->N() - 1) * (_calc->N() - 1), 3, _normales);
  _isInitialized = true;
}

void GW::_eraseArrays()
{
  _memfree(_calc->N2(), _pg);
  _memfree((_calc->N() - 1) * (_calc->N() - 1), _normales);
  _isInitialized = false;
}

void GW::_setPoints()
{
  _coefZ = (_calc->max(0) - _calc->min(0)) / (_calc->max(2) - _calc->min(2));
  //setting up points of axises
  for(int j = 0; j < 6; j++)
    for(int i = 0; i < 3; i++)
      _pa[j][i] = - (_calc->max(i) + _calc->min(i)) / 2.0;
  _pa[0][0] = _calc->min(0) - (_calc->max(0) + _calc->min(0)) / 2.0;
  _pa[1][0] = _calc->max(0) - (_calc->max(0) + _calc->min(0)) / 2.0;
  _pa[2][1] = _calc->min(1) - (_calc->max(1) + _calc->min(1)) / 2.0;
  _pa[3][1] = _calc->max(1) - (_calc->max(1) + _calc->min(1)) / 2.0;
  _pa[4][2] = _calc->min(2) - (_calc->max(2) + _calc->min(2)) / 2.0;
  _pa[5][2] = _calc->max(2) - (_calc->max(2) + _calc->min(2)) / 2.0;
  //setting up points of graphic
  int k = 0; //total counter
  for(int j = 0; j < _calc->N(); j++)
    for(int i = 0; i < _calc->N(); i++)
    {
      _pg[k][0] = _calc->x()[i] - (_calc->max(0) + _calc->min(0)) / 2.0;
      _pg[k][1] = _calc->y()[j] - (_calc->max(1) + _calc->min(1)) / 2.0;
      _pg[k][2] = _calc->z()[j][i] - (_calc->max(2) + _calc->min(2)) / 2.0;
      k++;
    }
  //setting up normales to faces
  k = 0;
  for(int j = 0; j < _calc->N() - 1; j++)
    for(int i = 0; i < _calc->N() - 1; i++)
    {
      _countNormale(_normales[k], _pg[_calc->N() * j + i],
                    _pg[_calc->N() * j + i + 1],
                    _pg[_calc->N() * (j + 1) + i + 1],
                    _pg[_calc->N() * (j + 1) + i]);
//      std::cout << k << "n = (" << _normales[k][0] << ';' << _normales[k][1] <<
//                   ';' << _normales[k][2] << ")\n";
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
  qglColor(Qt::gray);
  for(int i = 0; i < 6; i += 2)
  {
    glBegin(GL_LINES);
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

void GW::_paintLines()
{
  qglColor(Qt::red);
  glBegin(GL_LINES);
  for(int j = 0; j < _calc->N() - 1; j++)
    for(int i = 0; i < _calc->N(); i++)
    {
      glVertex3dv(_pg[j * _calc->N() + i]);
      glVertex3dv(_pg[(j + 1) * _calc->N() + i]);
      glVertex3dv(_pg[i * _calc->N() + j]);
      glVertex3dv(_pg[i * _calc->N() + j + 1]);
    }
  glEnd();
}

void GW::_paintFaces()
{
  qglColor(Qt::red);
  glBegin(GL_QUADS);
  int k = 0;
  for(int j = 0; j < _calc->N() - 1; j++)
    for(int i = 0; i < _calc->N() - 1; i++)
    {
//      glColor3d(1.0, 0.0, 0.0);
      glNormal3dv(_normales[k]);
      glVertex3dv(_pg[j * _calc->N() + i]);
      glVertex3dv(_pg[j * _calc->N() + i + 1]);
      glVertex3dv(_pg[(j + 1) * _calc->N() + i + 1]);
      glVertex3dv(_pg[(j + 1)* _calc->N() + i]);
      k++;
    }
  glEnd();
}

void GW::_countNormale(GLdouble *n, GLdouble *p1, GLdouble *p2, GLdouble *p3,
                            GLdouble *p4)
{
  GLdouble v1[3], v2[3];
  v1[0] = p3[0] - p1[0];
  v1[1] = p3[1] - p1[1];
  v1[2] = p3[2] - p1[2];
  v2[0] = p4[0] - p2[0];
  v2[1] = p4[1] - p2[1];
  v2[2] = p4[2] - p2[2];
//  std::cout << (v1[1] * v2[2] - v1[2] * v2[1]) << '\n';
//  std::cout << (v1[2] * v2[0] - v1[0] * v2[2]) << '\n';
//  std::cout << (v1[0] * v2[1] - v1[1] * v2[0]) << '\n';
  n[0] = (v1[1] * v2[2] - v1[2] * v2[1]);
  n[1] = (v1[2] * v2[0] - v1[0] * v2[2]);
  n[2] = (v1[0] * v2[1] - v1[1] * v2[0]);
//  std::cout << n[0] << '\n';
//  std::cout << n[1] << '\n';
//  std::cout << n[2] << '\n';
}
