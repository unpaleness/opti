#ifndef MW_H
#define MW_H

#include <QMainWindow>

namespace Ui { class MW; }

class GW;
class PW;
class Calc;

class MW : public QMainWindow
{
    Q_OBJECT

public:
    explicit MW(QMainWindow *parent = 0);
    ~MW();

    void showEvent(QShowEvent *);

    void setCalc(Calc *);
    void setGraphicForm(GW *);
    void setPlanarForm(PW *);
    int method();

private slots:
    void on_pbexec_clicked();
    void on_pbgraph_clicked();
    void on_cbMethod_currentIndexChanged(int);

private:
    Ui::MW *_mw; //main form
    GW *_gw; //graphic form
    PW *_pw; //planar from
    Calc *_calc; //calculating class

    int _method;

    int _readParams(); //record parameters from fields
    int _updateParams(); //update parameters (convert nulls and symbols to 0)
};

#endif // MW_H
