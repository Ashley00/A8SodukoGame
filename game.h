#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <iostream>
#include <QString>
#include <QVector>
#include <QProcess>
#include "model.h"
#include "numbers.h"
#include "scenewidget.h"
#include "gamewondialog.h"
#include "board.h"
#include "gameoverdialog.h"

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(Model& model,QWidget *parent = nullptr);
    ~Game();

signals:
    void sendPuzzleInput(int, int, int);
    void sendErase(int, int);

//    void reverseRedBoard(int, int);

    void sendInitBoard(int, std::vector<std::vector<int>>);
    void sendInitNumbers(int);
    void sendSetVector(int,int,int);
    void goToMenuPage();
    void sendInitModel(int);
    void sendInitExampleModel(int);
    void triggerBox2D();

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
    void on_backButton_clicked();

public slots:
    void receiveLevel4(int);
    void receiveLevel9(int);
    void receiveLevelExample(int);
    void receiveDisplayVector(std::vector<std::vector<int>>);


private:
    Ui::Game *ui;
    QVector<QVector<QLabel *>> cells;
    QVector<QLabel* > numbers;
    GameOverDialog *gameOver;
    gamewondialog *gameWon;
    SceneWidget* box2D;
    Board *board;
    Numbers *numbersWidget;
    int indexJ;
    int indexI;
    int mistakes;

    int level;

    std::vector<std::vector<int>> vector;

};

#endif // GAME_H
