#include <iostream>

#include "gw.h"
#include "ui_gw.h"
#include "calc.h"
#include "mw.h"
#include "pw.h"

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
  {
    _eraseArrays();
//    switch(_mw->method())
//    {
//      case 0: //simplex
//        _memfree(_calc->simplex()->nSimplex() * 3, _po);
//        break;
//      default:
//        break;
//    }
  }
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
//  glLightfv(GL_LIGHT0, GL_AMBIENT, lightDiffuse);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
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
    glScaled(1.0, 1.0,
             (_calc->max(0) - _calc->min(0)) /(_calc->max(2) - _calc->min(2)));
  glRotated(_rot[0] / 16.0, 1.0, 0.0, 0.0);
  glRotated(_rot[1] / 16.0, 0.0, 1.0, 0.0);
  glRotated(_rot[2] / 16.0, 0.0, 0.0, 1.0);
  switch(_graphView)
  {
    case 1:
      _paintLines();
      break;
    case 2:
      _paintFaces();
      break;
    default:
      _paintPoints();
      break;
  }
  _paintAxises();
//  _paintOptimization();
//  glFlush();
}

void GW::showEvent(QShowEvent *)
{
  if(!_isInitialized)
  {
    _initializeArrays();
//    switch(_mw->method())
//    {
//      case 0: //simplex
//        _memalloc(_calc->simplex()->nSimplex() * 3, 3, _po);
//        break;
//      default:
//        break;
//    }
  }
  _setPoints();
//  updateGL();
}

void GW::closeEvent(QCloseEvent *)
{
  _eraseArrays();
//  switch(_mw->method())
//  {
//    case 0: //simplex
//      _memfree(_calc->simplex()->nSimplex() * 3, _po);
//      break;
//    default:
//      break;
//  }
  _mw->setEnabled(true);
  if(!_pw->isHidden())
    _pw->close();
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
    _setRotation(0, _rot[0] + dy);
    _setRotation(1, _rot[1] + dx);
  }
  else if(event->buttons() & Qt::RightButton)
  {
    _setRotation(0, _rot[0] + dy);
    _setRotation(2, _rot[2] + dx);
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

void GW::setCalc(Calc *calc) { _calc = calc; }

void GW::setMainForm(MW *mw) { _mw = mw; }

void GW::setPlanarForm(PW *pw) { _pw = pw; }

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
  //setting up normales to faces to graphic
  k = 0;
  for(int j = 0; j < _calc->N() - 1; j++)
    for(int i = 0; i < _calc->N() - 1; i++)
    {
      _countNormale4(_normales[k], _pg[_calc->N() * j + i],
                    _pg[_calc->N() * j + i + 1],
                    _pg[_calc->N() * (j + 1) + i + 1],
                    _pg[_calc->N() * (j + 1) + i]);
      k++;
    }
  //setting up optimization points
//  switch(_mw->method())
//  {
//    case 0: //powell
//      break;
//    case 1: //simplex
//      for(int i = 0; i < _calc->simplex()->nSimplex() * 3; i++)
//      {
//        _po[i][0] = _calc->simplex()->simplex()[i / 3][i % 3][0] -
//            (_calc->max(0) + _calc->min(0)) / 2.0;
//        _po[i][1] = _calc->simplex()->simplex()[i / 3][i % 3][1] -
//            (_calc->max(1) + _calc->min(1)) / 2.0;
//        _po[i][2] = _calc->simplex()->simplex()[i / 3][i % 3][2] -
//            (_calc->max(2) + _calc->min(2)) / 2.0;

//      }
//      break;
//    default:
//      break;
//  }
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
  GLfloat materialEmission[4] = { 0.0, 0.0, 0.0, 1.0 };
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materialEmission);
  glColor3d(0.5, 0.5, 0.5);
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
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  GLfloat materialEmission[4] = { 0.0, 0.0, 0.0, 1.0 };
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materialEmission);
  glColor3d(1.0, 1.0, 1.0);
  glBegin(GL_POINTS);
  for(int i = 0; i < _calc->N2(); i++)
    glVertex3dv(_pg[i]);
  glEnd();
}

void GW::_paintLines()
{
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  GLfloat materialEmission[4] = { 0.0, 0.0, 0.0, 1.0 };
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materialEmission);
  glColor3d(1.0, 1.0, 1.0);
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
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  GLfloat materialEmission[4] = { 0.1, 0.0, 0.1, 1.0 };
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materialEmission);
  glBegin(GL_QUADS);
  int k = 0;
  for(int j = 0; j < _calc->N() - 1; j++)
    for(int i = 0; i < _calc->N() - 1; i++)
    {
      glNormal3dv(_normales[k]);
      glVertex3dv(_pg[j * _calc->N() + i]);
      glVertex3dv(_pg[j * _calc->N() + i + 1]);
      glVertex3dv(_pg[(j + 1) * _calc->N() + i + 1]);
      glVertex3dv(_pg[(j + 1)* _calc->N() + i]);
      k++;
    }
  glEnd();
}

//void GW::_paintOptimization()
//{
//  switch(_mw->method())
//  {
//    case 0: // powell
//      break;
//    case 1: //simplex
//      glBegin(GL_LINES);
//      for(int s = 0; s < _calc->simplex()->nSimplex() * 3; s += 3)
//      {
//        glColor3d(1.0 * (_calc->simplex()->nSimplex() / 3 - s) /
//                  _calc->simplex()->nSimplex() / 3,
//                  1.0 * s / _calc->simplex()->nSimplex() / 3, 0.0);
//        glVertex3dv(_po[s]);
//        glVertex3dv(_po[s + 1]);
//        glVertex3dv(_po[s + 1]);
//        glVertex3dv(_po[s + 2]);
//        glVertex3dv(_po[s + 2]);
//        glVertex3dv(_po[s]);
//      }
//      glEnd();
//      break;
//    default:
//      break;
//  }
//}

void GW::_countNormale4(GLdouble *n, GLdouble *p1, GLdouble *p2, GLdouble *p3,
                            GLdouble *p4)
{
  GLdouble v1[3], v2[3];
  v1[0] = p3[0] - p1[0];
  v1[1] = p3[1] - p1[1];
  v1[2] = p3[2] - p1[2];
  v2[0] = p4[0] - p2[0];
  v2[1] = p4[1] - p2[1];
  v2[2] = p4[2] - p2[2];
  n[0] = (v1[1] * v2[2] - v1[2] * v2[1]);
  n[1] = (v1[2] * v2[0] - v1[0] * v2[2]);
  n[2] = (v1[0] * v2[1] - v1[1] * v2[0]);
}
