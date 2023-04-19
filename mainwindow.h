#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <iostream>
#include <QString>
#include <QVector>
#include "model.h"
#include "gameoverdialog.h"

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
    void highlightCurrentRow(int);
    void highlightCurrentCol(int);
    void highlightCurrentSquare(int, int);

    void initialize();

signals:
    void sendPuzzleInput(int, int, int);
    void sendErase(int, int);
    //void sendAddNumber(int, int, int);

private slots:
    void receiveCorrectInput(int, int, int);
    void receiveIncorrectInput(int, int, int);
    void receiveWonGame();
    void receiveSecondChace();
    void receiveNewGame();
    void receiveTutorial();

    void on_eraseButton_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QVector<QLabel *>> cells;
    QVector<QLabel* > numbers;
    GameOverDialog *gameOver;
    int indexJ;
    int indexI;
    int mistakes;

    int level;

    std::vector<std::vector<int>> vector;

};
#endif // MAINWINDOW_H
