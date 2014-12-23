#include "mw.h"
#include "gw.h"
#include "pw.h"
#include "calc.h"
#include "ui_mw.h"

/*
 * PUBLIC
 */

MW::MW(QMainWindow *parent) : QMainWindow(parent), _mw(new Ui::MW)
{
  _method = 0;
  _mw->setupUi(this);
  _mw->cbMethod->setCurrentIndex(_method);
}

MW::~MW()
{
  delete _mw;
}

void MW::showEvent(QShowEvent *)
{
//  _mw->lestatus->setText(QString::number(int(_gw->isHidden())));
}

void MW::setCalc(Calc *calc) { _calc = calc; }

void MW::setGraphicForm(GW *gw) { _gw = gw; }

void MW::setPlanarForm(PW *pw) { _pw = pw; }

int MW::method() { return _method; }

/*
 * PRIVATE
 */

int MW::_readParams()
{
    _calc->a(0, 0) = _mw->lea1->text().toDouble();
    _calc->a(0, 1) = _mw->lea2->text().toDouble();
    _calc->a(0, 2) = _mw->lea3->text().toDouble();
    _calc->a(0, 3) = _mw->lea4->text().toDouble();
    _calc->a(0, 4) = _mw->lea5->text().toDouble();
    _calc->a(0, 5) = _mw->lea6->text().toDouble();
    _calc->powx(0, 0) = _mw->lepowx1->text().toDouble();
    _calc->powx(0, 1) = _mw->lepowx2->text().toDouble();
    _calc->powx(0, 2) = _mw->lepowx3->text().toDouble();
    _calc->powx(0, 3) = _mw->lepowx4->text().toDouble();
    _calc->powx(0, 4) = _mw->lepowx5->text().toDouble();
    _calc->powx(0, 5) = _mw->lepowx6->text().toDouble();
    _calc->powy(0, 0) = _mw->lepowy1->text().toDouble();
    _calc->powy(0, 1) = _mw->lepowy2->text().toDouble();
    _calc->powy(0, 2) = _mw->lepowy3->text().toDouble();
    _calc->powy(0, 3) = _mw->lepowy4->text().toDouble();
    _calc->powy(0, 4) = _mw->lepowy5->text().toDouble();
    _calc->powy(0, 5) = _mw->lepowy6->text().toDouble();
    _calc->a(1, 0) = _mw->lea1_2->text().toDouble();
    _calc->a(1, 1) = _mw->lea2_2->text().toDouble();
    _calc->a(1, 2) = _mw->lea3_2->text().toDouble();
    _calc->a(1, 3) = _mw->lea4_2->text().toDouble();
    _calc->a(1, 4) = _mw->lea5_2->text().toDouble();
    _calc->a(1, 5) = _mw->lea6_2->text().toDouble();
    _calc->powx(1, 0) = _mw->lepowx1_2->text().toDouble();
    _calc->powx(1, 1) = _mw->lepowx2_2->text().toDouble();
    _calc->powx(1, 2) = _mw->lepowx3_2->text().toDouble();
    _calc->powx(1, 3) = _mw->lepowx4_2->text().toDouble();
    _calc->powx(1, 4) = _mw->lepowx5_2->text().toDouble();
    _calc->powx(1, 5) = _mw->lepowx6_2->text().toDouble();
    _calc->powy(1, 0) = _mw->lepowy1_2->text().toDouble();
    _calc->powy(1, 1) = _mw->lepowy2_2->text().toDouble();
    _calc->powy(1, 2) = _mw->lepowy3_2->text().toDouble();
    _calc->powy(1, 3) = _mw->lepowy4_2->text().toDouble();
    _calc->powy(1, 4) = _mw->lepowy5_2->text().toDouble();
    _calc->powy(1, 5) = _mw->lepowy6_2->text().toDouble();
    _calc->a(2, 0) = _mw->lea1_3->text().toDouble();
    _calc->a(2, 1) = _mw->lea2_3->text().toDouble();
    _calc->a(2, 2) = _mw->lea3_3->text().toDouble();
    _calc->a(2, 3) = _mw->lea4_3->text().toDouble();
    _calc->a(2, 4) = _mw->lea5_3->text().toDouble();
    _calc->a(2, 5) = _mw->lea6_3->text().toDouble();
    _calc->powx(2, 0) = _mw->lepowx1_3->text().toDouble();
    _calc->powx(2, 1) = _mw->lepowx2_3->text().toDouble();
    _calc->powx(2, 2) = _mw->lepowx3_3->text().toDouble();
    _calc->powx(2, 3) = _mw->lepowx4_3->text().toDouble();
    _calc->powx(2, 4) = _mw->lepowx5_3->text().toDouble();
    _calc->powx(2, 5) = _mw->lepowx6_3->text().toDouble();
    _calc->powy(2, 0) = _mw->lepowy1_3->text().toDouble();
    _calc->powy(2, 1) = _mw->lepowy2_3->text().toDouble();
    _calc->powy(2, 2) = _mw->lepowy3_3->text().toDouble();
    _calc->powy(2, 3) = _mw->lepowy4_3->text().toDouble();
    _calc->powy(2, 4) = _mw->lepowy5_3->text().toDouble();
    _calc->powy(2, 5) = _mw->lepowy6_3->text().toDouble();
    _calc->lineEdits(0) = _mw->lineEdit_1->text().toDouble();
    _calc->lineEdits(1) = _mw->lineEdit_2->text().toDouble();
    _calc->lineEdits(2) = _mw->lineEdit_3->text().toDouble();
    _calc->lineEdits(3) = _mw->lineEdit_4->text().toDouble();
    _calc->lineEdits(4) = _mw->lineEdit_5->text().toDouble();
    _calc->lineEdits(5) = _mw->lineEdit_6->text().toDouble();
    _calc->lineEdits(6) = _mw->lineEdit_7->text().toDouble();
    _calc->lineEdits(7) = _mw->lineEdit_8->text().toDouble();
    _calc->lineEdits(8) = _mw->lineEdit_9->text().toDouble();
    _calc->lineEdits(9) = _mw->lineEdit_10->text().toDouble();
    _calc->setMin(0, _mw->leMinX->text().toDouble());
    _calc->setMax(0, _mw->leMaxX->text().toDouble());
    _calc->setMin(1, _mw->leMinY->text().toDouble());
    _calc->setMax(1, _mw->leMaxY->text().toDouble());
    _calc->initMethods(_mw->leAccuracy->text().toDouble(),
                       _mw->leIterations->text().toDouble());
    return 0;
}

int MW::_updateParams()
{
    _mw->lea1->setText(QString::number(_calc->a(0, 0)));
    _mw->lea2->setText(QString::number(_calc->a(0, 1)));
    _mw->lea3->setText(QString::number(_calc->a(0, 2)));
    _mw->lea4->setText(QString::number(_calc->a(0, 3)));
    _mw->lea5->setText(QString::number(_calc->a(0, 4)));
    _mw->lea6->setText(QString::number(_calc->a(0, 5)));
    _mw->lepowx1->setText(QString::number(_calc->powx(0, 0)));
    _mw->lepowx2->setText(QString::number(_calc->powx(0, 1)));
    _mw->lepowx3->setText(QString::number(_calc->powx(0, 2)));
    _mw->lepowx4->setText(QString::number(_calc->powx(0, 3)));
    _mw->lepowx5->setText(QString::number(_calc->powx(0, 4)));
    _mw->lepowx6->setText(QString::number(_calc->powx(0, 5)));
    _mw->lepowy1->setText(QString::number(_calc->powy(0, 0)));
    _mw->lepowy2->setText(QString::number(_calc->powy(0, 1)));
    _mw->lepowy3->setText(QString::number(_calc->powy(0, 2)));
    _mw->lepowy4->setText(QString::number(_calc->powy(0, 3)));
    _mw->lepowy5->setText(QString::number(_calc->powy(0, 4)));
    _mw->lepowy6->setText(QString::number(_calc->powy(0, 5)));
    _mw->lea1_2->setText(QString::number(_calc->a(1, 0)));
    _mw->lea2_2->setText(QString::number(_calc->a(1, 1)));
    _mw->lea3_2->setText(QString::number(_calc->a(1, 2)));
    _mw->lea4_2->setText(QString::number(_calc->a(1, 3)));
    _mw->lea5_2->setText(QString::number(_calc->a(1, 4)));
    _mw->lea6_2->setText(QString::number(_calc->a(1, 5)));
    _mw->lepowx1_2->setText(QString::number(_calc->powx(1, 0)));
    _mw->lepowx2_2->setText(QString::number(_calc->powx(1, 1)));
    _mw->lepowx3_2->setText(QString::number(_calc->powx(1, 2)));
    _mw->lepowx4_2->setText(QString::number(_calc->powx(1, 3)));
    _mw->lepowx5_2->setText(QString::number(_calc->powx(1, 4)));
    _mw->lepowx6_2->setText(QString::number(_calc->powx(1, 5)));
    _mw->lepowy1_2->setText(QString::number(_calc->powy(1, 0)));
    _mw->lepowy2_2->setText(QString::number(_calc->powy(1, 1)));
    _mw->lepowy3_2->setText(QString::number(_calc->powy(1, 2)));
    _mw->lepowy4_2->setText(QString::number(_calc->powy(1, 3)));
    _mw->lepowy5_2->setText(QString::number(_calc->powy(1, 4)));
    _mw->lepowy6_2->setText(QString::number(_calc->powy(1, 5)));
    _mw->lea1_3->setText(QString::number(_calc->a(2, 0)));
    _mw->lea2_3->setText(QString::number(_calc->a(2, 1)));
    _mw->lea3_3->setText(QString::number(_calc->a(2, 2)));
    _mw->lea4_3->setText(QString::number(_calc->a(2, 3)));
    _mw->lea5_3->setText(QString::number(_calc->a(2, 4)));
    _mw->lea6_3->setText(QString::number(_calc->a(2, 5)));
    _mw->lepowx1_3->setText(QString::number(_calc->powx(2, 0)));
    _mw->lepowx2_3->setText(QString::number(_calc->powx(2, 1)));
    _mw->lepowx3_3->setText(QString::number(_calc->powx(2, 2)));
    _mw->lepowx4_3->setText(QString::number(_calc->powx(2, 3)));
    _mw->lepowx5_3->setText(QString::number(_calc->powx(2, 4)));
    _mw->lepowx6_3->setText(QString::number(_calc->powx(2, 5)));
    _mw->lepowy1_3->setText(QString::number(_calc->powy(2, 0)));
    _mw->lepowy2_3->setText(QString::number(_calc->powy(2, 1)));
    _mw->lepowy3_3->setText(QString::number(_calc->powy(2, 2)));
    _mw->lepowy4_3->setText(QString::number(_calc->powy(2, 3)));
    _mw->lepowy5_3->setText(QString::number(_calc->powy(2, 4)));
    _mw->lepowy6_3->setText(QString::number(_calc->powy(2, 5)));
    _mw->lineEdit_1->setText(QString::number(_calc->lineEdits(0)));
    _mw->lineEdit_2->setText(QString::number(_calc->lineEdits(1)));
    _mw->lineEdit_3->setText(QString::number(_calc->lineEdits(2)));
    _mw->lineEdit_4->setText(QString::number(_calc->lineEdits(3)));
    _mw->lineEdit_5->setText(QString::number(_calc->lineEdits(4)));
    _mw->lineEdit_6->setText(QString::number(_calc->lineEdits(5)));
    _mw->lineEdit_7->setText(QString::number(_calc->lineEdits(6)));
    _mw->lineEdit_8->setText(QString::number(_calc->lineEdits(7)));
    _mw->lineEdit_9->setText(QString::number(_calc->lineEdits(8)));
    _mw->lineEdit_10->setText(QString::number(_calc->lineEdits(9)));
    _mw->leMinX->setText(QString::number(_calc->min(0)));
    _mw->leMaxX->setText(QString::number(_calc->max(0)));
    _mw->leMinY->setText(QString::number(_calc->min(1)));
    _mw->leMaxY->setText(QString::number(_calc->max(1)));
    return 0;
}

void MW::on_pbexec_clicked()
{
    QString str = "Ошибок нет.";
    _readParams();
    _updateParams();
    _calc->countPlot();
    _calc->optimize(_mw->cbMethod->currentIndex(),
                    _mw->cbExtremum->currentIndex());
    if(_mw->cbExtremum->currentIndex() == 0)
      str = "Найден максимум в точке x* = ";
    else
      str = "Найден минимум в точке x* = ";
    str += QString::number(_calc->getExtremum(_method)[0]) + ", y* = "
        + QString::number(_calc->getExtremum(_method)[1]) + ": z* = "
        + QString::number(_calc->f(_calc->getExtremum(_method)[0],
                   _calc->getExtremum(_method)[1])) + " - за "
        + QString::number(_calc->getNPoints(_method)) + " итераций";
    _mw->lestatus->setText(str);
//    _mw->lestatus->setText(QString::number(_calc->simplex()->nSimplex()));
}

void MW::on_pbgraph_clicked()
{
  if(_calc->isCounted())
  {
    if(_mw->checkBox3D->isChecked())
      _gw->show();
    _pw->doShowIterationNumbers(_mw->checkBoxIterations->isChecked());
    _pw->show();
    this->setEnabled(false);
  }
  else
  {
    _mw->lestatus->setText("График не рассчитан.");
  }
}

void MW::on_cbMethod_currentIndexChanged(int index)
{
  _method = index;
}
