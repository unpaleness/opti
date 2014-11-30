#include "gw.h"
#include "ui_gw.h"

GW::GW(QGLWidget *parent) : QGLWidget(parent), _gw(new Ui::GW)
{
  _gw->setupUi(this);
}

GW::~GW()
{
  delete _gw;
}

void GW::mainform(MW *mw) { _mw = mw; }

void GW::calcobject(Calc *calc) { _calc = calc; }
