#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // square grid widget
    QGridLayout *gridLayout = new QGridLayout(ui->squareWidget);
    gridLayout->setSpacing(0);
    //gridLayout->setContentsMargins(20, 20, 20, 20);

    // number widget
    QGridLayout *numberLayout = new QGridLayout(ui->numberWidget);
    numberLayout->setSpacing(5);

    // Create a 4x4 grid of square labels
    for(int i = 0; i < 4; i++) {
        QVector<QLabel *> row;
        for(int j = 0; j < 4; j++) {
            QLabel *squareLabel = new QLabel(ui->squareWidget);
            squareLabel->setFrameStyle(QFrame::Box);
            if(model.displayVector[i][j] != 0)
            squareLabel->setText(QString::number(model.displayVector[i][j]));
            QFont font("Arial", 20);
            squareLabel->setFont(font);
            squareLabel->setAlignment(Qt::AlignCenter);
            gridLayout->addWidget(squareLabel, i, j);
            row.push_back(squareLabel);
        }
        cells.push_back(row);
    }
    //test
    cells[0][3]->setText("0");

    // create 1*9 grid for input numbers
    for(int i = 0; i < 4; i++){
        QLabel *numberLabel = new QLabel(ui->numberWidget);
        numberLabel->setFrameStyle(QFrame::Box);
        numberLayout->addWidget(numberLabel, 0, i);
        QFont font("Arial", 20);
        numberLabel->setFont(font);
        numberLabel->setAlignment(Qt::AlignCenter);
        numberLabel->setText(QString::number(i+1));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

