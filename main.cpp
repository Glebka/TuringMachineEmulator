#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.connect(&a,&QApplication::focusChanged,&w,&MainWindow::onFocusChanged);
    w.show();
    return a.exec();
}
