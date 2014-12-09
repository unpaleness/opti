#include "mw.h"
#include "gw.h"
#include "calc.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calc calc;
    GW g;
    MW w;
    w.setCalc(&calc);
    g.setCalc(&calc);
    w.setGraphicForm(&g);
    g.setMainForm(&w);
    w.show();

    return a.exec();
}
