#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameoverdialog.h"
#include "board.h"
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QProcess>

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->winGame->setVisible(false);

    // initialize index with -1
    indexI = -1;
    indexJ = -1;
    mistakes = 0;

    /* Board Widget */
    board = new Board(ui->boardWidget);
    connect(this, &MainWindow::sendInitBoard, board, &Board::receiveBoard);
    connect(board, &Board::sendCells, this, &MainWindow::receiveCells);
    connect(board, &Board::sendBoxSelected, this, &MainWindow::receiveBoxSelected);
    connect(this, &MainWindow::sendSetVector, board, &Board::receiveVector);

    /* Numbers Widget */
    numbersWidget = new Numbers(ui->numbersWidget);
    connect(this, &MainWindow::sendInitNumbers, numbersWidget, &Numbers::receiveInitNumbers);
    connect(numbersWidget, &Numbers::sendNumberClicked, this, &MainWindow::receiveNumberClicked);
    connect(numbersWidget, &Numbers::sendNumbers, this, &MainWindow::receiveNumbers);
    numbersWidget->setVisible(true);

    /* Game Over Widget */
    gameOver = new GameOverDialog(ui->gameOverDialog);
    connect(gameOver, &GameOverDialog::sendSecondChance, this, &MainWindow::receiveSecondChace);
    connect(gameOver, &GameOverDialog::sendNewGame, this, &MainWindow::receiveNewGame);
    connect(gameOver, &GameOverDialog::sendTutorial, this, &MainWindow::receiveTutorial);

    level = 4;

    vector = {{0,0,0,0},
              {0,0,0,0},
              {0,0,0,0},
              {0,0,0,0}};

    /* Erase Button */
    ui->eraseButton->setIcon(QIcon(":/images/eraser.png"));
    ui->eraseButton->setIconSize(QSize(60,60));
    ui->eraseButton->setStyleSheet("QPushButton {border-radius: 10px; border: 1px solid black;}");
    connect(ui->eraseButton, &QPushButton::clicked, this, &MainWindow::eraseButtonClicked);
    connect(this, &MainWindow::sendErase, &model, &Model::receiveErase);
    //connect(this, &MainWindow::reverseRedBoard, board, &Board::setBackColor);

    /* Undo Button */
    ui->undoButton->setIcon(QIcon(":/images/undo.png"));
    ui->undoButton->setIconSize(QSize(60,60));
    ui->undoButton->setStyleSheet("QPushButton {border-radius: 10px; border: 1px solid black;}");
   // connect(this, &MainWindow::sendUndo, &model, &Model::receiveErase);

    /* Notes Button */
    ui->notesButton->setIcon(QIcon(":/images/notes.png"));
    ui->notesButton->setIconSize(QSize(60,60));
    ui->notesButton->setStyleSheet("QPushButton {border-radius: 10px; border: 1px solid black;}");
   // connect(this, &MainWindow::sendUndo, &model, &Model::receiveErase);


    /* Model */
    connect(this, &MainWindow::sendPuzzleInput, &model, &Model::receivePuzzleInput);
    connect(&model, &Model::sendCorrectInput, this, &MainWindow::receiveCorrectInput);
    connect(&model, &Model::sendIncorrectInput, this, &MainWindow::receiveIncorrectInput);
    connect(&model, &Model::sendWonGame, this, &MainWindow::receiveWonGame);


    emit(sendInitBoard(level, model.displayVector));
    emit(sendInitNumbers(level));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receiveBoxSelected(int j, int i)
{
    indexJ = j;
    indexI = i;
}

void MainWindow::receiveNumbers(QVector<QLabel *> nums)
{
    numbers = nums;
}

void MainWindow::receiveNumberClicked(QString num)
{
    if((indexI != -1 && indexJ != -1) && cells[indexJ][indexI]->isEnabled()){
        cells[indexJ][indexI]->setText(num);
        sendPuzzleInput(num.toInt(), indexJ, indexI);
    }

}

void MainWindow::receiveCorrectInput(int input, int indexJ, int indexI)
{
    qDebug() << "Correct Input Received!";
    //background-color is light blue which highlight the current box, and set the correct number to green
    cells[indexJ][indexI]->setStyleSheet("QLabel {background-color : rgba(173, 216, 230, 128); color: rgba(0, 165, 11, 1);}");
    cells[indexJ][indexI]->setText(numbers[input - 1]->text());
    vector[indexJ][indexI] = 1;
    emit sendSetVector(indexJ, indexI, 1);
}

void MainWindow::receiveIncorrectInput(int input, int indexJ, int indexI)
{
    qDebug() << "Input: " << input;
    mistakes++;
    qDebug() << "IndexJ: " << indexJ;
    qDebug() << "IndexI: " << indexI;
    cells[indexJ][indexI]->setStyleSheet("QLabel {background-color : rgba(173, 216, 230, 128); color: red;}");
    qDebug() << "Reached Here ";
    qDebug() << "Numbers size " << numbers.size();
    cells[indexJ][indexI]->setText(numbers[input - 1]->text());

    vector[indexJ][indexI] = 2;
    emit sendSetVector(indexJ,indexI,2);
    ui->mistakesLabel->setText(QString("Mistakes: %1/5").arg(mistakes));
    if(mistakes == 5){

        gameOver->setVisible(true);
    }
}

/**
 * @brief MainWindow::shonkWin
 * When win the game, show congratulation message
 */
void MainWindow::receiveWonGame()
{
    qDebug() << "Game Won!";
    ui->winGame->setVisible(true);

}

void MainWindow::receiveSecondChace()
{
    qDebug() << "Mistakes before second chance: " << mistakes;
    mistakes--;
    qDebug() << "Mistakes after second chance: " << mistakes;
    ui->mistakesLabel->setText(QString("Mistakes: %1/5").arg(mistakes));
    gameOver->setVisible(false);

}

void MainWindow::receiveNewGame()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::receiveTutorial()
{

}

void MainWindow::receiveCells(QVector<QVector<QLabel *>> cells_)
{
    cells = cells_;
    board->setVisible(true);
}

/**
 * @brief MainWindow::on_eraseButton_clicked
 * When click the erase button, remove the current selected number.
 * If nothing has been selected, click this button won't work
 */
void MainWindow::eraseButtonClicked()
{
    qDebug() << "cells[indexJ][indexI]->isEnabled()";
    if(indexI != -1 && indexJ != -1)
    {
        qDebug() << cells[indexJ][indexI]->isEnabled();
        if(cells[indexJ][indexI]->isEnabled())
        {

            emit sendErase(indexJ, indexI);

            cells[indexJ][indexI]->setText("");

            emit reverseRedBoard(indexJ, indexI);
        }
    }
}

