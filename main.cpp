#include "mainwindow.h"
#include "model.h"
#include <QApplication>

/**
 * @brief qMain
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model m;
    MainWindow w(m);
    w.show();
    return a.exec();
}
