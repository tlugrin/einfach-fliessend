#include <QApplication>
#include "fluessigmain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("fusion");
    FluessigMain w;
    w.show();
    return a.exec();
}
