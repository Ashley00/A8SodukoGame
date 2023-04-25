#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <iostream>
#include <QString>
#include <QVector>
#include "model.h"
#include "numbers.h"
#include "board.h"
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

signals:
    void sendPuzzleInput(int, int, int);
    void sendErase(int, int);

    void reverseRedBoard(int, int);

    void sendInitBoard(int, std::vector<std::vector<int>>);
    void sendInitNumbers(int);
    void sendSetVector(int,int,int);

private slots:
    void receiveBoxSelected(int, int);
    void receiveNumbers(QVector<QLabel *>);
    void receiveNumberClicked(QString);
    void receiveCorrectInput(int, int, int);
    void receiveIncorrectInput(int, int, int);
    void receiveWonGame();
    void receiveSecondChace();
    void receiveNewGame();
    void receiveTutorial();
    void receiveCells(QVector<QVector<QLabel *>>);
    void eraseButtonClicked();

private:
    Ui::MainWindow *ui;
    QVector<QVector<QLabel *>> cells;
    QVector<QLabel* > numbers;
    GameOverDialog *gameOver;
    Board *board;
    Numbers *numbersWidget;
    int indexJ;
    int indexI;
    int mistakes;

    int level;

    std::vector<std::vector<int>> vector;

};
#endif // MAINWINDOW_H
