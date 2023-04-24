#include "board.h"
#include "ui_board.h"
#include <QDebug>
#include <QMouseEvent>

Board::Board(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Board)
{
    ui->setupUi(this);
    level = 4;
    vector = {{0,0,0,0},
              {0,0,0,0},
              {0,0,0,0},
              {0,0,0,0}};
}

void Board::receiveBoard(int level, std::vector<std::vector<int>> displayVector)
{
    level = 4;

    /* Sudoku Board Widget */
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);

    // Create a 4x4 grid of square labels
    for(int i = 0; i < level; i++) {
        QVector<QLabel *> row;
        for(int j = 0; j < level; j++) {
            QLabel *squareLabel = new QLabel(this);
            squareLabel->setFrameStyle(QFrame::Box);

            if(displayVector[i][j] != 0)
                squareLabel->setText(QString::number(displayVector[i][j]));

            // disable label with fixed number
            if(!squareLabel->text().isEmpty()){
                QPalette palette = squareLabel->palette();
                palette.setColor(QPalette::Disabled, QPalette::Base, Qt::white); // set transparent for disabled background
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
    emit(sendCells(cells));
}

void Board::receiveVector(int i, int j, int input)
{
    vector[i][j] = input;
}

void Board::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    setTransparent();
    for(int i = 0; i < level; i++){
        for(int j = 0; j < level; j++){
            QLabel *currLabel = cells[j][i];
            int width = currLabel->width();
            int height = currLabel->height();
            int labelX = currLabel->x();
            int labelY = currLabel->y();
            if((x >= labelX) && (x <= labelX + width) && (y >= labelY) && (y <= labelY + height)){
                highlightCurrentSquare(j, i);
                highlightCurrentRow(j);
                highlightCurrentCol(i);
                setLightBlue(j, i);
                emit(sendBoxSelected(j, i));
            }
        }
    }
}
void Board::setLightBlue(int j, int i){
     cells[j][i]->setStyleSheet("QLabel { background-color : rgba(173, 216, 230, 128);}");
}

void Board::setDarkBlue(int j, int i){
     cells[j][i]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26);}");
}
void Board::setTransparent(){
    for(int i = 0; i < level; i++){
        for(int j = 0; j < level; j++){
             cells[i][j]->setStyleSheet("QLabel { background-color : rgba(0, 0, 0, 0);}");
//            setRedGreen(i, j);
            if(vector[i][j] == 1){
                cells[i][j]->setStyleSheet("QLabel { background-color : rgba(0, 0, 0, 0); color: rgba(0, 165, 11, 1);}");
            }
            if(vector[i][j] == 2){
                cells[i][j]->setStyleSheet("QLabel { background-color : rgba(244, 194, 194, 128); color: red;}");
            }
        }
    }
}

Board::~Board()
{
    delete ui;
}

void Board::highlightCurrentRow(int indexJ)
{
    for(int col = 0; col < level; col++){
       setDarkBlue(indexJ, col);
       setRedGreen(indexJ, col);
    }
}

void Board::highlightCurrentCol(int indexI)
{
    for(int row = 0; row < level; row++){
        setDarkBlue(row, indexI);
        setRedGreen(row, indexI);
    }
}

void Board::highlightCurrentSquare(int indexJ, int indexI)
{
    if((indexJ >= 0 && indexJ <= 1) && (indexI >= 0 && indexI <= 1)){
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++){
                setDarkBlue(i, j);
                setRedGreen(i, j);
            }
    }else if((indexJ >= 2 && indexJ <= 3) && (indexI >= 0 && indexI <= 1)){
        for(int i = 2; i < 4; i++)
            for(int j = 0; j < 2; j++){
                setDarkBlue(i, j);
                setRedGreen(i, j);
            }
    }else if((indexJ >= 0 && indexJ <= 1) && (indexI >= 2 && indexI <= 3)){
        for(int i = 0; i < 2; i++)
            for(int j = 2; j < 4; j++){
                setDarkBlue(i, j);
                setRedGreen(i, j);
            }
    }else if((indexJ >= 2 && indexJ <= 3) && (indexI >= 2 && indexI <= 3)){
        for(int i = 2; i < 4; i++)
            for(int j = 2; j < 4; j++){
                setDarkBlue(i, j);
                setRedGreen(i, j);
            }
    }
}

void Board::setRedGreen(int i, int j){
    if(vector[i][j] == 1)
        cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: rgba(0, 165, 11, 1);}");
    if(vector[i][j] == 2)
        cells[i][j]->setStyleSheet("QLabel { background-color : rgba(46, 90, 154, 0.26); color: red;}");
}
