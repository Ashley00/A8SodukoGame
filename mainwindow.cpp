#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QGridLayout>


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
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            QLabel *squareLabel = new QLabel(ui->squareWidget);
            squareLabel->setFrameStyle(QFrame::Box);
            gridLayout->addWidget(squareLabel, i, j);
        }
    }

    // create 1*9 grid for input numbers
    for(int i = 0; i < 9; i++){
        QLabel *numberLabel = new QLabel(ui->numberWidget);
        numberLabel->setFrameStyle(QFrame::Box);
        numberLayout->addWidget(numberLabel, 0, i);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

