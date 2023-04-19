#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameoverdialog.h"
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

    /* Sudoku Board Widget */
    QGridLayout *gridLayout = new QGridLayout(ui->squareWidget);
    gridLayout->setSpacing(0);
    //gridLayout->setContentsMargins(20, 20, 20, 20);
    /* Number bank Widget */
    QGridLayout *numberLayout = new QGridLayout(ui->numberWidget);
    numberLayout->setSpacing(5);
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

    // Create a 4x4 grid of square labels
    for(int i = 0; i < level; i++) {
        QVector<QLabel *> row;
        for(int j = 0; j < level; j++) {
            QLabel *squareLabel = new QLabel(ui->squareWidget);
            squareLabel->setFrameStyle(QFrame::Box);

            if(model.displayVector[i][j] != 0)
                squareLabel->setText(QString::number(model.displayVector[i][j]));

            // disable label with fixed number
            if(!squareLabel->text().isEmpty()){
                QPalette palette = squareLabel->palette();
                palette.setColor(QPalette::Disabled, QPalette::Base, Qt::transparent); // set transparent for disabled background
                palette.setColor(QPalette::Disabled, QPalette::WindowText, Qt::black); // set black for disabled text
                squareLabel->setPalette(palette);
                squareLabel->setEnabled(false);
            }
            QFont font("Arial", 20);
            squareLabel->setFont(font);
            squareLabel->setAlignment(Qt::AlignCenter);
            gridLayout->addWidget(squareLabel, i, j);
            row.push_back(squareLabel);
        }
        cells.push_back(row);
    }

    // create 1*4 grid for input numbers
    for(int i = 0; i < level; i++){
        QLabel *numberLabel = new QLabel(ui->numberWidget);
        numberLabel->setFrameStyle(QFrame::Box);
        numberLayout->addWidget(numberLabel, 0, i);
        QFont font("Arial", 20);
        numberLabel->setFont(font);
        numberLabel->setAlignment(Qt::AlignCenter);
        numberLabel->setText(QString::number(i+1));
        numbers.push_back(numberLabel);
    }

    // set the erase button
    ui->eraseButton->setIcon(QIcon(":/images/eraser.png"));
    ui->eraseButton->setIconSize(QSize(80,80));

    connect(this,
            &MainWindow::sendErase,
            &model,
            &Model::receiveErase);
    //connect(this,&MainWindow::sendAddNumber,&model,&Model::receiveAdd);

    /******** BEGIN CONNECTIONS ********/

    /* Main Window --> Model */
    connect(this, &MainWindow::sendPuzzleInput, &model, &Model::receivePuzzleInput);

    /* Model --> Main Window */
    connect(&model, &Model::sendCorrectInput, this, &MainWindow::receiveCorrectInput);
    connect(&model, &Model::sendIncorrectInput, this, &MainWindow::receiveIncorrectInput);
    connect(&model, &Model::sendWonGame, this, &MainWindow::receiveWonGame);

    /******** END CONNECTIONS ********/
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent * event) {
    if(event->button() == Qt::LeftButton){
        int x = event->x();
        int y = event->y();

        bool isInSquare = (x>200 && x<700) && (y>20 && y<520);
        bool isInNumbers = (x>200 && x<700) && (y>540 && y<600);
        if(isInSquare){
            int tempX = 325;
            for(int i = 0; i < 4; i++){
                if(x<tempX){
                    indexI = i;
                    //qDebug() << "i" << i-1;
                    break;
                }
                tempX = tempX + 125;
            }
            int tempY = 145;
            for(int j = 0; j < level; j++){
                if(y<tempY){
                    indexJ = j;
                    //qDebug() << "j" << j-1;
                    break;
                }
                tempY = tempY + 125;
            }
            for(int i = 0; i < level; i++){
                for(int j = 0; j < level; j++){
                    //transparent color
//                    if(vector[i][j] != 0)
                        cells[i][j]->setStyleSheet("QLabel { background-color : rgba(0, 0, 0, 0);}");
                        if(vector[i][j] == 1){
                            cells[i][j]->setStyleSheet("QLabel { background-color : rgba(0, 0, 0, 0); color: rgba(0, 165, 11, 1);}");
                        }
                        if(vector[i][j] == 2){
                            cells[i][j]->setStyleSheet("QLabel { background-color : rgba(244, 194, 194, 128); color: red;}");
                        }
                }
            }
            highlightCurrentSquare(indexJ, indexI);
            highlightCurrentRow(indexJ);
            highlightCurrentCol(indexI);
            //light blue color
            cells[indexJ][indexI]->setStyleSheet("QLabel { background-color : rgba(173, 216, 230, 128);}");
            //qDebug() << "square indexJ: " << indexJ << " square indexI: " << indexI;

        }
        else if(isInNumbers){
            int tempX = 325;
            int indexINumber = -1;
            for(int i = 0; i < level; i++){
                if(x<tempX){
                    indexINumber = i;
                    //qDebug() << "number i:" << i-1;
                    break;
                }
                tempX = tempX + 125;
            }
            //qDebug() << "indexJ: " << indexJ << " indexI: " << indexI;
            if(cells[indexJ][indexI]->isEnabled()){
                QColor selectedColor = cells[indexJ][indexI]->palette().color(QPalette::Base);
                QColor lightblue = QColor(173, 216, 230, 128);
                if(selectedColor == lightblue){
                    emit sendPuzzleInput(indexINumber, indexJ, indexI);
                    //cells[indexJ][indexI]->setText(numbers[indexINumber]->text());
                }
            }
        }
    }
//    for(int i = 0; i < 4; i++){
//        for(int j = 0; j < 4; j++){
//            if(vector[i][j] == 1){
//                cells[i][j]->setStyleSheet("color: rgba(0, 165, 11, 1);}");
//            }
//            if(vector[i][j] == 2){
//                cells[i][j]->setStyleSheet("color: red;}");
//            }
//        }
//    }
}

void MainWindow::highlightCurrentRow(int indexJ)
{
    for(int col = 0; col < level; col++){
       //dark blue that highlight the whole row
       cells[indexJ][col]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); }");
       if(vector[indexJ][col] == 1){
           cells[indexJ][col]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: rgba(0, 165, 11, 1);}");
       }
       if(vector[indexJ][col] == 2){
           cells[indexJ][col]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: red;}");
       }
    }
//    for(int i = 0; i < 4; i++){
//        for(int j = 0; j < 4; j++){
//            if(vector[i][j] == 1){
//                cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: rgba(0, 165, 11, 1);}");
//            }
//            if(vector[i][j] == 2){
//                cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: red;}");
//            }
//        }
//    }
}

void MainWindow::highlightCurrentCol(int indexI)
{
    for(int row = 0; row < level; row++){
        //dark blue that highlight the whole column
        cells[row][indexI]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); }");
        if(vector[row][indexI] == 1){
            cells[row][indexI]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: rgba(0, 165, 11, 1);}");
        }
        if(vector[row][indexI] == 2){
            cells[row][indexI]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: red;}");
        }
    }
}

void MainWindow::highlightCurrentSquare(int indexJ, int indexI)
{
    //dark blue that highlight the whole square
    QString color = "QLabel { background-color : rgba(46, 90, 154, 0.26); }";
    if((indexJ >= 0 && indexJ <= 1) && (indexI >= 0 && indexI <= 1)){
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                cells[i][j]->setStyleSheet(color);
                if(vector[i][j] == 1){
                    cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: rgba(0, 165, 11, 1);}");
                }
                if(vector[i][j] == 2){
                    cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: red;}");
                }
            }
        }
    }else if((indexJ >= 2 && indexJ <= 3) && (indexI >= 0 && indexI <= 1)){
        for(int i = 2; i < 4; i++){
            for(int j = 0; j < 2; j++){
                cells[i][j]->setStyleSheet(color);
                if(vector[i][j] == 1){
                    cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: rgba(0, 165, 11, 1);}");
                }
                if(vector[i][j] == 2){
                    cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: red;}");
                }
            }
        }
    }else if((indexJ >= 0 && indexJ <= 1) && (indexI >= 2 && indexI <= 3)){
        for(int i = 0; i < 2; i++){
            for(int j = 2; j < 4; j++){
                cells[i][j]->setStyleSheet(color);
                if(vector[i][j] == 1){
                    cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: rgba(0, 165, 11, 1);}");
                }
                if(vector[i][j] == 2){
                    cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: red;}");
                }
            }
        }
    }else if((indexJ >= 2 && indexJ <= 3) && (indexI >= 2 && indexI <= 3)){
        for(int i = 2; i < 4; i++){
            for(int j = 2; j < 4; j++){
                cells[i][j]->setStyleSheet(color);
                if(vector[i][j] == 1){
                    cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: rgba(0, 165, 11, 1);}");
                }
                if(vector[i][j] == 2){
                    cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: red;}");
                }
            }
        }
    }




//    if((indexJ == 0 && indexI == 0) || (indexJ == 2 && indexI == 0) || (indexJ == 0 && indexI == 2) || (indexJ == 2 && indexI == 2) ){
//        cells[indexJ][indexI + 1]->setStyleSheet(color);
//        cells[indexJ + 1][indexI]->setStyleSheet(color);
//        cells[indexJ + 1][indexI + 1]->setStyleSheet(color);
//    } else if((indexJ == 0 && indexI == 1) || (indexJ == 0 && indexI == 3) || (indexJ == 2 && indexI == 1) || (indexJ == 2 && indexI == 3)){
//        cells[indexJ][indexI - 1]->setStyleSheet(color);
//        cells[indexJ + 1][indexI]->setStyleSheet(color);
//        cells[indexJ + 1][indexI - 1]->setStyleSheet(color);
//    } else if((indexJ == 1 && indexI == 1) || (indexJ == 1 && indexI == 3) || (indexJ == 3 && indexI == 1) || (indexJ == 3 && indexI == 3)){
//        cells[indexJ][indexI - 1]->setStyleSheet(color);
//        cells[indexJ - 1][indexI -1]->setStyleSheet(color);
//        cells[indexJ - 1][indexI]->setStyleSheet(color);
//    } else if((indexJ == 1 && indexI == 0) || (indexJ == 1 && indexI == 2) || (indexJ == 3 && indexI == 0) || (indexJ == 3 && indexI == 2)){
//        cells[indexJ - 1][indexI]->setStyleSheet(color);
//        cells[indexJ - 1][indexI  + 1]->setStyleSheet(color);
//        cells[indexJ][indexI + 1]->setStyleSheet(color);
//    }
}

void MainWindow::initialize()
{

}

void MainWindow::receiveCorrectInput(int input, int indexJ, int indexI)
{
    qDebug() << "Correct Input Received!";
    //background-color is light blue which highlight the current box, and set the correct number to green
    cells[indexJ][indexI]->setStyleSheet("QLabel {background-color : rgba(173, 216, 230, 128); color: rgba(0, 165, 11, 1);}");
    cells[indexJ][indexI]->setText(numbers[input]->text());
    vector[indexJ][indexI] = 1;
}

void MainWindow::receiveIncorrectInput(int input, int indexJ, int indexI)
{
    qDebug() << "Mistakes before user input: " << mistakes;
    mistakes++;
     qDebug() << "Mistakes after user input: " << mistakes;
    cells[indexJ][indexI]->setStyleSheet("QLabel {background-color : rgba(173, 216, 230, 128); color: red;}");
    cells[indexJ][indexI]->setText(numbers[input]->text());
    vector[indexJ][indexI] = 2;
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

/**
 * @brief MainWindow::on_eraseButton_clicked
 * When click the erase button, remove the current selected number.
 * If nothing has been selected, click this button won't work
 */
void MainWindow::on_eraseButton_clicked()
{
    if(indexI != -1 && indexJ != -1)
    {
        if(cells[indexJ][indexI]->isEnabled())
        {
            emit sendErase(indexJ, indexI);

            cells[indexJ][indexI]->setText("");
        }
    }
}

