#include "kcmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KCMainWindow w;
    w.show();

    return a.exec();
}
