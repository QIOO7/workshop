#include "workshopwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    workshopWidget w;
    w.show();
    return a.exec();
}
