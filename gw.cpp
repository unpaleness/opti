#include "gw.h"
#include "mw.h"
#include "calc.h"
#include "ui_gw.h"

/*
 * PUBLIC
 */

GW::GW(QWidget *parent) : QWidget(parent), _gw(new Ui::GW)
{
  _initialized = false;
  _phi = acos(-1) / 10;
  _psi = acos(-1) / 6;
  _scale = 8.0;
  _perspective_half_z = 300;
  _eye_point.z = 100;
  _gw->setupUi(this);
  _gw->lepoints->setText(QString::number(POINTS));
}

GW::~GW()
{
  if(_initialized)
  {
    _eraseinitialization();
  }
  delete _gw;
}

void GW::calcobject(Calc *calc) { _calc = calc; }

void GW::mainform(MW *mw) { _mw = mw; }

/*
 * EVENTS
 */

void GW::mousePressEvent(QMouseEvent *me)
{
  _mouse_pos_x = me->pos().x();
  _mouse_pos_y = me->pos().y();
}

void GW::mouseMoveEvent(QMouseEvent *me)
{
  if(abs(me->pos().x()-_mouse_pos_x) > CRIT_COORDS_X ||
     abs(me->pos().y()-_mouse_pos_y) > CRIT_COORDS_Y)
  {
    _phi -= (me->pos().x()-_mouse_pos_x)*MULTI_COORDS_TO_ANGLE;
    _psi -= (me->pos().y()-_mouse_pos_y)*MULTI_COORDS_TO_ANGLE;
    _scale_init_points();
    _rotate_points();
    update();
    _mouse_pos_x = me->pos().x();
    _mouse_pos_y = me->pos().y();
  }
}

void GW::wheelEvent(QWheelEvent *we)
{
  if(we->spontaneous())
  {
    if(we->delta() > 0 && _scale < MAX_SCALE) //forwards away from the user
    {
      _scale *= MULTI_SCALE;
      _scale_points(MULTI_SCALE);
    }
    if(we->delta() < 0 && _scale > MIN_SCALE) //backwards toward the user
    {
      _scale /= MULTI_SCALE;
      _scale_points(1.0 / MULTI_SCALE);
    }
    update();
  }
}

void GW::showEvent(QShowEvent *)
{
  if(!_initialized)
  {
    _initialize();
    //resize windows
    this->setGeometry(0, 0, WINDOW_SIZE, WINDOW_SIZE);
    //fill lineedits
    _gw->lemaxx->setText(QString::number(_calc->maxx()));
    _gw->leminx->setText(QString::number(_calc->minx()));
    _gw->lemaxy->setText(QString::number(_calc->maxy()));
    _gw->leminy->setText(QString::number(_calc->miny()));
  }
  _initialized = true;
  _scale_init_points();
  _rotate_points();
}

void GW::paintEvent(QPaintEvent *)
{
  switch(_gw->cbgraphstyle->currentIndex())
  {
    case 1:
      _paint_axises();
      _paint_lines();
      break;
    case 2:
      _paint_faces();
      break;
    default:
      _paint_axises();
      _paint_points();
      break;
  }
}

void GW::closeEvent(QCloseEvent *)
{
  _mw->setEnabled(true);
}

/*
 * PRIVATE SLOTS
 */

void GW::on_pbapplychangepoints_clicked()
{
  int newN = _gw->lepoints->text().toInt();
  if(newN != _calc->N())
  {
    if(newN < MINPOINTS) newN = MINPOINTS;
    if(newN > MAXPOINTS) newN = MAXPOINTS;
    _eraseinitialization();
    _calc->setN(newN);
    _initialize();
    _scale_init_points();
    _rotate_points();
    update();
  }
}

void GW::on_pbapplyaxis_clicked()
{
  bool mistakes = false;
  if(_gw->lemaxx->text().toDouble() <= _gw->leminx->text().toDouble())
  {
    mistakes = true;
    _gw->leminx->setText("=(");
    _gw->lemaxx->setText("=(");
  }
  if(_gw->lemaxy->text().toDouble() <= _gw->leminy->text().toDouble())
  {
    mistakes = true;
    _gw->leminy->setText("=(");
    _gw->lemaxy->setText("=(");
  }
  if(!mistakes)
  {
    _calc->setMaxX(_gw->lemaxx->text().toDouble());
    _calc->setMinX(_gw->leminx->text().toDouble());
    _calc->setMaxY(_gw->lemaxy->text().toDouble());
    _calc->setMinY(_gw->leminy->text().toDouble());
    _calc->countPlot();
    _scale_init_points();
    _rotate_points();
    update();
  }
}

void GW::on_cbgraphstyle_currentIndexChanged(int)
{
  update();
}

void GW::on_cbzscale_stateChanged(int)
{
  _scale_init_points();
  _rotate_points();
  update();
}

/*
 * PRIVATE
 */

void GW::_initialize()
{
  /* memory for points and faces and its order (faces')*/
  _p = new Point *[_calc->N()];
  for(int i = 0; i < _calc->N(); i++)
    _p[i] = new Point [_calc->N()];
  _f = new Face [(_calc->N()-1)*(_calc->N()-1)];
  _face_order = new int [(_calc->N()-1)*(_calc->N()-1)];
  /* connecting points and faces */
  int fnum = 0;
  for(int j = 0; j < _calc->N()-1; j++)
    for(int i = 0; i < _calc->N()-1; i++)
    {
      _f[fnum].p[0] = &_p[j][i];
      _f[fnum].p[1] = &_p[j][i+1];
      _f[fnum].p[2] = &_p[j+1][i+1];
      _f[fnum].p[3] = &_p[j+1][i];
      _face_order[fnum] = fnum;
      fnum++;
    }
}

void GW::_eraseinitialization()
{
  for(int i = 0; i < _calc->N(); i++)
    delete [] _p[i];
  delete [] _p;
  delete [] _f;
  delete [] _face_order;
}

void GW::_rotate(Point &p, double z, double y, double x)
{
  p.x = cos(_phi)*x-sin(_phi)*y;
  p.y = cos(_psi)*(sin(_phi)*x+cos(_phi)*y)-sin(_psi)*z;
  p.z = sin(_psi)*(sin(_phi)*x+cos(_phi)*y)+cos(_psi)*z;
  p.x *= pow(2, -(_eye_point.z - p.z)/_perspective_half_z);
  p.y *= pow(2, -(_eye_point.z - p.z)/_perspective_half_z);
}

void GW::_scale_init_points()
{
  //count center point
  _center_point.x = (_calc->maxx()+_calc->minx())/2;
  _center_point.y = (_calc->maxy()+_calc->miny())/2;
  _center_point.z = (_calc->maxz()+_calc->minz())/2;
  //make surface more suitable for analysis by normalizing z-coordinate
  if(_gw->cbzscale->isChecked())
    _scale_z = WINDOW_SIZE / (_calc->maxz()-_calc->minz()) / _scale;
  else
    _scale_z = 1;
  //
  for(int j = 0; j < _calc->N(); j++)
    for(int i = 0; i < _calc->N(); i++)
    {
      _p[j][i].x = (_calc->x()[i]-_center_point.x) * _scale;
      _p[j][i].y = (_calc->y()[j]-_center_point.y) * _scale;
      _p[j][i].z = (_calc->z()[j][i]-_center_point.z) * _scale * _scale_z;
    }
}

void GW::_scale_points(double multi)
{
  for(int j = 0; j < _calc->N(); j++)
    for(int i = 0; i < _calc->N(); i++)
    {
      _p[j][i].x *= multi;
      _p[j][i].y *= multi;
      _p[j][i].z *= multi;
    }
}

void GW::_rotate_points()
{
  for(int j = 0; j < _calc->N(); j++)
    for(int i = 0; i < _calc->N(); i++)
      _rotate(_p[j][i], _p[j][i].z, _p[j][i].y, _p[j][i].x);
}

void GW::_paint_axises()
{
  Point p1, p2;
  QPainter painter(this);
  painter.setPen(Qt::black);
  //Oz axis
  p1.x = 0-_center_point.x*_scale; p1.y = 0-_center_point.y*_scale; p1.z = -WINDOW_SIZE/2-_center_point.z*_scale*_scale_z;
  p2.x = 0-_center_point.x*_scale; p2.y = 0-_center_point.y*_scale; p2.z = WINDOW_SIZE/2-_center_point.z*_scale*_scale_z;
  _rotate(p1, p1.z, p1.y, p1.x);
  _rotate(p2, p2.z, p2.y, p2.x);
  painter.drawLine(p1.x+WINDOW_SIZE/2, p1.y+WINDOW_SIZE/2,
                   p2.x+WINDOW_SIZE/2, p2.y+WINDOW_SIZE/2);
  //Oy axis
  p1.x = 0-_center_point.x*_scale; p1.y = -WINDOW_SIZE/2-_center_point.y*_scale; p1.z = 0-_center_point.z*_scale*_scale_z;
  p2.x = 0-_center_point.x*_scale; p2.y = WINDOW_SIZE/2-_center_point.y*_scale; p2.z = 0-_center_point.z*_scale*_scale_z;
  _rotate(p1, p1.z, p1.y, p1.x);
  _rotate(p2, p2.z, p2.y, p2.x);
  painter.drawLine(p1.x+WINDOW_SIZE/2, p1.y+WINDOW_SIZE/2,
                   p2.x+WINDOW_SIZE/2, p2.y+WINDOW_SIZE/2);
  //Ox axis
  p1.x = -WINDOW_SIZE/2-_center_point.x*_scale; p1.y = 0-_center_point.y*_scale; p1.z = 0-_center_point.z*_scale*_scale_z;
  p2.x = WINDOW_SIZE/2-_center_point.x*_scale; p2.y = 0-_center_point.y*_scale; p2.z = 0-_center_point.z*_scale*_scale_z;
  _rotate(p1, p1.z, p1.y, p1.x);
  _rotate(p2, p2.z, p2.y, p2.x);
  painter.drawLine(p1.x+WINDOW_SIZE/2, p1.y+WINDOW_SIZE/2,
                   p2.x+WINDOW_SIZE/2, p2.y+WINDOW_SIZE/2);
  //legend
  p1.x = 0-_center_point.x*_scale; p1.y = 0-_center_point.y*_scale; p1.z = 0-_center_point.z*_scale*_scale_z;
  _rotate(p1, p1.z, p1.y, p1.x);
  painter.drawText(p1.x+WINDOW_SIZE/2, p1.y+WINDOW_SIZE/2, "0");
  p1.x = WINDOW_SIZE * 0.48-_center_point.x*_scale; p1.y = 0-_center_point.y*_scale; p1.z = 0-_center_point.z*_scale*_scale_z;
  _rotate(p1, p1.z, p1.y, p1.x);
  painter.drawText(p1.x+WINDOW_SIZE/2, p1.y+WINDOW_SIZE/2, "x");
  p1.x = 0-_center_point.x*_scale; p1.y = WINDOW_SIZE * 0.48-_center_point.y*_scale; p1.z = 0-_center_point.z*_scale*_scale_z;
  _rotate(p1, p1.z, p1.y, p1.x);
  painter.drawText(p1.x+WINDOW_SIZE/2, p1.y+WINDOW_SIZE/2, "y");
  p1.x = 0-_center_point.x*_scale; p1.y = 0-_center_point.y*_scale; p1.z = WINDOW_SIZE * 0.48-_center_point.z*_scale*_scale_z;
  _rotate(p1, p1.z, p1.y, p1.x);
  painter.drawText(p1.x+WINDOW_SIZE/2, p1.y+WINDOW_SIZE/2, "z");
}

void GW::_paint_points()
{
  QPainter painter(this);
  painter.setPen(Qt::black);
  for(int j = 0; j < _calc->N(); j++)
    for(int i = 0; i < _calc->N(); i++)
      painter.drawPoint(_p[j][i].x+WINDOW_SIZE/2, _p[j][i].y+WINDOW_SIZE/2);
}

void GW::_paint_lines()
{
  QPainter painter(this);
  painter.setPen(Qt::black);
  for(int j = 0; j < _calc->N() - 1; j++)
    for(int i = 0; i < _calc->N(); i++)
    {
      painter.drawLine(_p[j][i].x+WINDOW_SIZE/2, _p[j][i].y+WINDOW_SIZE/2,
                       _p[j+1][i].x+WINDOW_SIZE/2, _p[j+1][i].y+WINDOW_SIZE/2);
      painter.drawLine(_p[i][j].x+WINDOW_SIZE/2, _p[i][j].y+WINDOW_SIZE/2,
                       _p[i][j+1].x+WINDOW_SIZE/2, _p[i][j+1].y+WINDOW_SIZE/2);
    }
}

void GW::_paint_faces()
{
  //count a coord of center point for each face
  for(int i = 0; i < (_calc->N()-1)*(_calc->N()-1); i++)
    _f[i].c.z = (_f[i].p[0]->z+_f[i].p[1]->z+_f[i].p[1]->z+_f[i].p[1]->z)/4;
  //sorting faces in increasing z coord order
  int temp;
  for(int j = 0; j < (_calc->N()-1)*(_calc->N()-1); j++)
    for(int i = j; i < (_calc->N()-1)*(_calc->N()-1); i++)
      if(_f[_face_order[i]].c.z < _f[_face_order[j]].c.z)
      {
        temp = _face_order[j];
        _face_order[j] = _face_order[i];
        _face_order[i] = temp;
      }
  //painting
  QPainter painter(this);
  QPoint p[4];
  QBrush b;
  b.setStyle(Qt::SolidPattern);
  b.setColor(Qt::magenta);
  painter.setPen(Qt::black);
  painter.setBrush(b);
  for(int i = 0; i < (_calc->N()-1)*(_calc->N()-1); i++)
  {
    p[0].setX(_f[_face_order[i]].p[0]->x+WINDOW_SIZE/2);
    p[0].setY(_f[_face_order[i]].p[0]->y+WINDOW_SIZE/2);
    p[1].setX(_f[_face_order[i]].p[1]->x+WINDOW_SIZE/2);
    p[1].setY(_f[_face_order[i]].p[1]->y+WINDOW_SIZE/2);
    p[2].setX(_f[_face_order[i]].p[2]->x+WINDOW_SIZE/2);
    p[2].setY(_f[_face_order[i]].p[2]->y+WINDOW_SIZE/2);
    p[3].setX(_f[_face_order[i]].p[3]->x+WINDOW_SIZE/2);
    p[3].setY(_f[_face_order[i]].p[3]->y+WINDOW_SIZE/2);
    painter.drawPolygon(p, 4);
  }
}
