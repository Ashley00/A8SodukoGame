#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <iostream>
#include <QString>
#include <QVector>
#include <QGridLayout>

namespace Ui {
class Board;
}

class Board : public QWidget
{
    Q_OBJECT

public:
    explicit Board(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *);
    void highlightCurrentRow(int);
    void highlightCurrentCol(int);
    void highlightCurrentSquare(int, int);
    void setTransparent();
    void setRedGreen(int, int);
    void setLightBlue(int, int);
    void setDarkBlue(int, int);
    void showLines();
    //void setBackColor(int, int);

    ~Board();

public slots:
    void receiveBoard(int, std::vector<std::vector<int>>);
    void receiveVector(int,int,int);

signals:
    void sendCells(QVector<QVector<QLabel *>>);
     void sendBoxSelected(int, int);
private:
    Ui::Board *ui;
    QVector<QVector<QLabel *>> cells;
    QVector<QLabel* > numbers;
    std::vector<std::vector<int>> vector;
    int level;
};

#endif // BOARD_H
