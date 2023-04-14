#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <iostream>
#include <QString>
#include <QVector>
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model& model, QWidget *parent = nullptr);
    ~MainWindow();

    void mousePressEvent(QMouseEvent *);

private:
    Ui::MainWindow *ui;
    QVector<QVector<QLabel *>> cells;
    QVector<QLabel* > numbers;

    int indexJ;
    int indexI;
};
#endif // MAINWINDOW_H
