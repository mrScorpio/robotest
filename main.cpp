#include "robotest.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    robotest w;
    w.show();
    return a.exec();
}
