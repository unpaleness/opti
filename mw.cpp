#include "mw.h"
#include "gw.h"
#include "calc.h"
#include "ui_mw.h"

/*
 * PUBLIC
 */

MW::MW(QMainWindow *parent) : QMainWindow(parent), _mw(new Ui::MW)
{
    _mw->setupUi(this);
}

MW::~MW()
{
  delete _mw;
}

void MW::calcobject(Calc *calc) { _calc = calc; }

void MW::graphicform(GW *gw) { _gw = gw; }

/*
 * PRIVATE
 */

int MW::_readParams()
{
    _calc->a(0) = _mw->lea1->text().toDouble();
    _calc->a(1) = _mw->lea2->text().toDouble();
    _calc->a(2) = _mw->lea3->text().toDouble();
    _calc->powx(0) = _mw->lepowx1->text().toDouble();
    _calc->powx(1) = _mw->lepowx2->text().toDouble();
    _calc->powx(2) = _mw->lepowx3->text().toDouble();
    _calc->powy(0) = _mw->lepowy1->text().toDouble();
    _calc->powy(1) = _mw->lepowy2->text().toDouble();
    _calc->powy(2) = _mw->lepowy3->text().toDouble();
    return 0;
}

int MW::_updateParams()
{
    _mw->lea1->setText(QString::number(_calc->a(0)));
    _mw->lea2->setText(QString::number(_calc->a(1)));
    _mw->lea3->setText(QString::number(_calc->a(2)));
    _mw->lepowx1->setText(QString::number(_calc->powx(0)));
    _mw->lepowx2->setText(QString::number(_calc->powx(1)));
    _mw->lepowx3->setText(QString::number(_calc->powx(2)));
    _mw->lepowy1->setText(QString::number(_calc->powy(0)));
    _mw->lepowy2->setText(QString::number(_calc->powy(1)));
    _mw->lepowy3->setText(QString::number(_calc->powy(2)));
    return 0;
}

void MW::on_pbexec_clicked()
{
    QString str = "Ошибок нет.";
    _readParams();
    _updateParams();
    _mw->lestatus->setText(str);
    _calc->countPlot();
}

void MW::on_pbgraph_clicked()
{
  if(_calc->isCounted())
  {
    _gw->show();
    this->setEnabled(false);
  }
  else
  {
    _mw->lestatus->setText("График не рассчитан.");
  }
}
