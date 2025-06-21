#include "mainwindow.h"

#include <QApplication>

#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    LoginDialog d;
    d.show();
    w.show();
    return a.exec();
}
