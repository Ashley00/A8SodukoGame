#include "game.h"
#include "ui_game.h"

Game::Game(Model& model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);

    ui->winGame->setVisible(false);

    // initialize index with -1
    indexI = -1;
    indexJ = -1;
    mistakes = 0;

    /* Board Widget */
    board = new Board(ui->boardWidget);
    connect(this, &Game::sendInitBoard, board, &Board::receiveBoard);
    connect(board, &Board::sendCells, this, &Game::receiveCells);
    connect(board, &Board::sendBoxSelected, this, &Game::receiveBoxSelected);
    connect(this, &Game::sendSetVector, board, &Board::receiveVector);

    /* Numbers Widget */
    numbersWidget = new Numbers(ui->numbersWidget);
    connect(this, &Game::sendInitNumbers, numbersWidget, &Numbers::receiveInitNumbers);
    connect(numbersWidget, &Numbers::sendNumberClicked, this, &Game::receiveNumberClicked);
    connect(numbersWidget, &Numbers::sendNumbers, this, &Game::receiveNumbers);
    numbersWidget->setVisible(true);

    /* Game Over Widget */
    gameOver = new GameOverDialog(ui->gameOverDialog);
    connect(gameOver, &GameOverDialog::sendSecondChance, this, &Game::receiveSecondChace);
    connect(gameOver, &GameOverDialog::sendNewGame, this, &Game::receiveNewGame);
    connect(gameOver, &GameOverDialog::sendTutorial, this, &Game::receiveTutorial);

    level = 0;

    /* Erase Button */
    ui->eraseButton->setIcon(QIcon(":/images/eraser.png"));
    ui->eraseButton->setIconSize(QSize(60,60));
    ui->eraseButton->setStyleSheet("QPushButton {border-radius: 10px; border: 1px solid black;}");
    connect(ui->eraseButton, &QPushButton::clicked, this, &Game::eraseButtonClicked);
    connect(this, &Game::sendErase, &model, &Model::receiveErase);
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
    connect(this, &Game::sendPuzzleInput, &model, &Model::receivePuzzleInput);
    connect(&model, &Model::sendCorrectInput, this, &Game::receiveCorrectInput);
    connect(&model, &Model::sendIncorrectInput, this, &Game::receiveIncorrectInput);
    connect(&model, &Model::sendWonGame, this, &Game::receiveWonGame);
    connect(&model, &Model::sendDispplayVector, this, &Game::receiveDisplayVector);
    /* Send Init Model */
     connect(this, &Game::sendInitModel, &model, &Model::receiveInitModel);

}

Game::~Game()
{
    delete ui;
}

void Game::receiveBoxSelected(int j, int i)
{
    indexJ = j;
    indexI = i;
}

void Game::receiveNumbers(QVector<QLabel *> nums)
{
    numbers = nums;
}

void Game::receiveNumberClicked(QString num)
{
    if((indexI != -1 && indexJ != -1) && cells[indexJ][indexI]->isEnabled()){
        cells[indexJ][indexI]->setText(num);
        sendPuzzleInput(num.toInt(), indexJ, indexI);
    }

}

void Game::receiveCorrectInput(int input, int indexJ, int indexI)
{
    qDebug() << "Correct Input Received!";
    //background-color is light blue which highlight the current box, and set the correct number to green
    cells[indexJ][indexI]->setStyleSheet("QLabel {background-color : rgba(173, 216, 230, 128); color: rgba(0, 165, 11, 1);}");
    cells[indexJ][indexI]->setText(numbers[input - 1]->text());
    vector[indexJ][indexI] = 1;
    emit sendSetVector(indexJ, indexI, 1);
}

void Game::receiveIncorrectInput(int input, int indexJ, int indexI)
{
    mistakes++;
    cells[indexJ][indexI]->setStyleSheet("QLabel {background-color : rgba(173, 216, 230, 128); color: red;}");
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
void Game::receiveWonGame()
{
    qDebug() << "Game Won!";
    ui->winGame->setVisible(true);

}

void Game::receiveSecondChace()
{
    qDebug() << "Mistakes before second chance: " << mistakes;
    mistakes--;
    qDebug() << "Mistakes after second chance: " << mistakes;
    ui->mistakesLabel->setText(QString("Mistakes: %1/5").arg(mistakes));
    gameOver->setVisible(false);

}

void Game::receiveNewGame()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void Game::receiveTutorial()
{

}

void Game::receiveCells(QVector<QVector<QLabel *>> cells_)
{
    cells = cells_;
    board->setVisible(true);
}

/**
 * @brief MainWindow::on_eraseButton_clicked
 * When click the erase button, remove the current selected number.
 * If nothing has been selected, click this button won't work
 */
void Game::eraseButtonClicked()
{
    qDebug() << "cells[indexJ][indexI]->isEnabled()";
    if(indexI != -1 && indexJ != -1)
    {
        qDebug() << cells[indexJ][indexI]->isEnabled();
        if(cells[indexJ][indexI]->isEnabled())
        {

            emit sendErase(indexJ, indexI);

            cells[indexJ][indexI]->setText("");
            sendSetVector(indexJ,indexI,0);
        }
    }
}

void Game::on_backButton_clicked()
{
    this->hide();
    emit goToMenuPage();
}

void Game::receiveLevel4(int level_)
{
    level = level_;
    vector =  std::vector<std::vector<int>>(level, std::vector<int>(level, 0));
    emit(sendInitModel(4));
    this->show();
}

void Game::receiveLevel9(int level_)
{
    level = level_;
    vector =  std::vector<std::vector<int>>(level, std::vector<int>(level, 0));
    emit(sendInitModel(9));
    this->show();
}

void Game::receiveDisplayVector(std::vector<std::vector<int>> dv)
{
     emit(sendInitBoard(level, dv));
     emit(sendInitNumbers(level));
}

