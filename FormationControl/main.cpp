#include "formationcontrol.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormationControl w;
    w.setWindowTitle("Formation Control Demo");
    w.showMaximized();
    return a.exec();
}
