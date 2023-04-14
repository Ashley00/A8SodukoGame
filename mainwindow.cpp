#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QDebug>

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
    for(int i = 0; i < 4; i++){
        QLabel *numberLabel = new QLabel(ui->numberWidget);
        numberLabel->setFrameStyle(QFrame::Box);
        numberLayout->addWidget(numberLabel, 0, i);
        QFont font("Arial", 20);
        numberLabel->setFont(font);
        numberLabel->setAlignment(Qt::AlignCenter);
        numberLabel->setText(QString::number(i+1));
        numbers.push_back(numberLabel);
    }
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
            for(int j = 0; j < 4; j++){
                if(y<tempY){
                    indexJ = j;
                    //qDebug() << "j" << j-1;
                    break;
                }
                tempY = tempY + 125;
            }

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    cells[i][j]->setStyleSheet("QLabel { background-color : rgba(0, 0, 0, 0); }");
                }
            }
            cells[indexJ][indexI]->setStyleSheet("QLabel { background-color : rgba(173, 216, 230, 128); }");
            //qDebug() << "square indexJ: " << indexJ << " square indexI: " << indexI;
        }
        else if(isInNumbers){
            int tempX = 325;
            int indexINumber;
            for(int i = 0; i < 4; i++){
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
                    cells[indexJ][indexI]->setText(numbers[indexINumber]->text());
                }
            }
        }
    }
}
