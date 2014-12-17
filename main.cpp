#include "mw.h"
#include "gw.h"
#include "pw.h"
#include "calc.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calc calc;
    GW g;
    MW w;
    PW p;
    w.setCalc(&calc);
    w.setGraphicForm(&g);
    w.setPlanarForm(&p);
    g.setCalc(&calc);
    g.setMainForm(&w);
    g.setPlanarForm(&p);
    p.setCalc(&calc);
    p.setMainForm(&w);
    p.setGraphicForm(&g);
    w.show();

    return a.exec();
}
