#include "pw.h"
#include "ui_pw.h"
#include "mw.h"
#include "gw.h"
#include "calc.h"

PW::PW(QGLWidget *parent) : QGLWidget(parent), _pw(new Ui::PW)
{
  _isInitialized = false;
  _gridLinesX = 9;
  _gridLinesY = 9;
  _levelLines = 100;
  _pw->setupUi(this);
}

PW::~PW()
{
  if(_isInitialized)
  {
    for(int i = 0; i < _calc->N2Planar(); i++)
      delete [] _pg[i];
    delete [] _pg;
  }
  delete _pw;
}

void PW::showEvent(QShowEvent *)
{
  if(!_isInitialized)
  {
    _pg = new GLdouble *[_calc->N2Planar()];
    for(int i = 0; i < _calc->N2Planar(); i++)
      _pg[i] = new GLdouble [3];
    _isInitialized = true;
  }
  _setPoints();
}

void PW::closeEvent(QCloseEvent *)
{
  _mw->setEnabled(true);
  if(!_gw->isHidden())
    _gw->close();
}

void PW::initializeGL()
{
  qglClearColor(Qt::white);
}

void PW::resizeGL(int w, int h)
{
  int side = qMin(w, h);
  glViewport((w-side)/2, (h-side)/2, side, side);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(_calc->min(0), _calc->max(0), _calc->min(1), _calc->max(1),
          _calc->min(2), _calc->max(2));
  glMatrixMode(GL_MODELVIEW);
}

void PW::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity(); //загружаем матрицу
  _paintPoints();
  _paintAxises();
  _paintWay();
}

void PW::setCalc(Calc *calc) { _calc = calc; }
void PW::setMainForm(MW *mw) { _mw = mw; }
void PW::setGraphicForm(GW *gw) { _gw = gw; }
void PW::doShowIterationNumbers(bool b) { _doShowIterationNumbers = b; }

void PW::_setPoints()
{
  int k = 0;
  for(int j = 0; j < _calc->NPlanar(); j++)
    for(int i = 0; i < _calc->NPlanar(); i++)
    {
      _pg[k][0] = _calc->xP()[i];
      _pg[k][1] = _calc->yP()[j];
      _pg[k][2] = _calc->zP()[j][i];
      k++;
    }
}

void PW::_paintPoints()
{
  glBegin(GL_QUADS);
  GLdouble zRange = _calc->max(2) - _calc->min(2);
  GLdouble zMin = _calc->min(2);
  GLdouble zMax = _calc->max(2);
  for(int i = 0; i < _calc->N2Planar(); i++)
  {
    if(int((_pg[i][2] - zMin) / zRange * _levelLines) % 2)
      glColor3d((_pg[i][2] - zMin) / zRange * 0.7,
                (zMax - _pg[i][2]) / zRange * 0.7, 0.0);
    else
      glColor3d((zMax - _pg[i][2]) / zRange * 0.7, 0.0,
                (_pg[i][2] - zMin) / zRange * 0.7);
//    glVertex3dv(_pg[i]);
    glVertex2d(_pg[i][0] - _calc->step(0) / 2.0,
               _pg[i][1] - _calc->step(1) / 2.0);
    glVertex2d(_pg[i][0] - _calc->step(0) / 2.0,
               _pg[i][1] + _calc->step(1) / 2.0);
    glVertex2d(_pg[i][0] + _calc->step(0) / 2.0,
               _pg[i][1] + _calc->step(1) / 2.0);
    glVertex2d(_pg[i][0] + _calc->step(0) / 2.0,
               _pg[i][1] - _calc->step(1) / 2.0);
  }
  glEnd();
}

void PW::_paintAxises()
{
  glBegin(GL_LINES);
  // x-grid
  glColor3d(0.3, 0.3, 0.3);
  for(int i = 1; i < _gridLinesY + 1; i++)
  {
    glVertex2d(_calc->min(0), _calc->min(1) +
               (_calc->max(1) - _calc->min(1)) * i / (_gridLinesY + 1));
    glVertex2d(_calc->max(0), _calc->min(1) +
               (_calc->max(1) - _calc->min(1)) * i / (_gridLinesY + 1));
  }
  // x-axis
  glColor3d(0.0, 0.0, 0.0);
  glVertex2d(_calc->min(0), 0.0);
  glVertex2d(_calc->max(0), 0.0);
  // x-arrow
  glVertex2d(_calc->max(0), 0.0);
  glVertex2d(_calc->max(0) - (_calc->max(0) - _calc->min(0)) * 0.05,
             (_calc->max(1) - _calc->min(1)) * 0.025);
  glVertex2d(_calc->max(0), 0.0);
  glVertex2d(_calc->max(0) - (_calc->max(0) - _calc->min(0)) * 0.05,
             - (_calc->max(1) - _calc->min(1)) * 0.025);
  // y-grid
  glColor3d(0.3, 0.3, 0.3);
  for(int i = 1; i < _gridLinesX + 1; i++)
  {
    glVertex2d(_calc->min(0) + (_calc->max(0) -
               _calc->min(0)) * i / (_gridLinesY + 1), _calc->min(1));
    glVertex2d(_calc->min(0) + (_calc->max(0) -
               _calc->min(0)) * i / (_gridLinesY + 1), _calc->max(1));
  }
  // y-axis
  glColor3d(0.0, 0.0, 0.0);
  glVertex2d(0.0, _calc->min(1));
  glVertex2d(0.0, _calc->max(1));
  // y-arrow
  glVertex2d(0.0, _calc->max(1));
  glVertex2d((_calc->max(0) - _calc->min(0)) * 0.025,
             _calc->max(1) - (_calc->max(1) - _calc->min(1)) * 0.05);
  glVertex2d(0.0, _calc->max(1));
  glVertex2d(- (_calc->max(0) - _calc->min(0)) * 0.025,
             _calc->max(1) - (_calc->max(1) - _calc->min(1)) * 0.05);
  glEnd();  
  // text
  glColor3d(0.2, 0.2, 0.2);
  renderText(- (_calc->max(0) - _calc->min(0)) * 0.025,
             _calc->max(1) - (_calc->max(1) - _calc->min(1)) * 0.05,
             0.0, "y");
  renderText(_calc->max(0) - (_calc->max(0) - _calc->min(0)) * 0.05,
             - (_calc->max(1) - _calc->min(1)) * 0.025,
             0.0, "x");
  for(int i = 1; i < _gridLinesY + 1; i++)
    renderText(- (_calc->max(0) - _calc->min(0)) * 0.025, _calc->min(1) +
               (_calc->max(1) - _calc->min(1)) * i / (_gridLinesY + 1),
               0.0, QString::number(_calc->min(1) + (_calc->max(1) -
               _calc->min(1)) * i / (_gridLinesY + 1)));
  for(int i = 1; i < _gridLinesX + 1; i++)
    renderText(_calc->min(0) + (_calc->max(0) - _calc->min(0)) * i /
               (_gridLinesX + 1), - (_calc->max(1) - _calc->min(1)) * 0.025,
               0.0, QString::number(_calc->min(1) + (_calc->max(1) -
               _calc->min(1)) * i / (_gridLinesY + 1)));
}

void PW::_paintWay()
{
  GLdouble colorLines[3] = {1.0, 0.0, 1.0};
  GLdouble colorText[3] = {1.0, 0.0, 1.0};
  switch(_mw->method())
  {
    case 0: // powell
      glColor3dv(colorLines);
      glBegin(GL_LINES);
      for(int i = 0; i < _calc->method(0)->nPoints() - 1; i++)
      {
        glVertex2d(_calc->method(0)->points(i, 0),
                   _calc->method(0)->points(i, 1));
        glVertex2d(_calc->method(0)->points(i + 1, 0),
                   _calc->method(0)->points(i + 1, 1));
      }
      glEnd();
      if(_doShowIterationNumbers)
      {
        glColor3dv(colorText);
        for(int i = 0; i < _calc->method(0)->nPoints(); i++)
          renderText(_calc->method(0)->points(i, 0),
                     _calc->method(0)->points(i, 1), 0.0, QString::number(i));
      }
      break;
    case 1: // fletcher-reeves
      glColor3dv(colorLines);
      glBegin(GL_LINES);
      for(int i = 0; i < _calc->method(1)->nPoints() - 1; i++)
      {
        glVertex2d(_calc->method(1)->points(i, 0),
                   _calc->method(1)->points(i, 1));
        glVertex2d(_calc->method(1)->points(i + 1, 0),
                   _calc->method(1)->points(i + 1, 1));
      }
      glEnd();
      if(_doShowIterationNumbers)
      {
        glColor3dv(colorText);
        for(int i = 0; i < _calc->method(1)->nPoints(); i++)
          renderText(_calc->method(1)->points(i, 0),
                     _calc->method(1)->points(i, 1), 0.0, QString::number(i));
      }
      break;
    case 2: // nelder-mid
      glColor3dv(colorLines);
      glBegin(GL_LINES);
      for(int i = 0; i < _calc->method(2)->nPoints() - 4; i += 3)
      {
        glVertex2d(_calc->method(2)->points(i, 0),
                   _calc->method(2)->points(i, 1));
        glVertex2d(_calc->method(2)->points(i + 1, 0),
                   _calc->method(2)->points(i + 1, 1));
        glVertex2d(_calc->method(2)->points(i + 1, 0),
                   _calc->method(2)->points(i + 1, 1));
        glVertex2d(_calc->method(2)->points(i + 2, 0),
                   _calc->method(2)->points(i + 2, 1));
        glVertex2d(_calc->method(2)->points(i + 2, 0),
                   _calc->method(2)->points(i + 2, 1));
        glVertex2d(_calc->method(2)->points(i, 0),
                   _calc->method(2)->points(i, 1));
      }
      glEnd();
      if(_doShowIterationNumbers)
      {
        glColor3dv(colorText);
        for(int i = 0; i < _calc->method(2)->nPoints() - 4; i += 3)
          renderText((_calc->method(2)->points(i, 0)
                      + _calc->method(2)->points(i + 1, 0)
                      + _calc->method(2)->points(i + 2, 0)) / 3,
                     (_calc->method(2)->points(i, 1)
                      + _calc->method(2)->points(i + 1, 1)
                      + _calc->method(2)->points(i + 2, 1)) / 3,
                     0.0, QString::number(i / 3));
      }
      break;
    case 3: // gradient descent
      glColor3dv(colorLines);
      glBegin(GL_LINES);
      for(int i = 0; i < _calc->method(3)->nPoints() - 1; i++)
      {
        glVertex2d(_calc->method(3)->points(i, 0),
                   _calc->method(3)->points(i, 1));
        glVertex2d(_calc->method(3)->points(i + 1, 0),
                   _calc->method(3)->points(i + 1, 1));
      }
      glEnd();
      if(_doShowIterationNumbers)
      {
        glColor3dv(colorText);
        for(int i = 0; i < _calc->method(3)->nPoints(); i++)
          renderText(_calc->method(3)->points(i, 0),
                     _calc->method(3)->points(i, 1), 0.0, QString::number(i));
      }
      break;
    default:
      break;
  }

}
