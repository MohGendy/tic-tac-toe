#include "mainwindow.h"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    qInfo() << "started";
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int ret = a.exec();
    qInfo() << "Event loop exited, code =" << ret;
    return ret;
}
