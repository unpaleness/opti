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
    w.calcobject(&calc);
    g.calcobject(&calc);
    w.graphicform(&g);
    g.mainform(&w);
    w.show();

    return a.exec();
}
