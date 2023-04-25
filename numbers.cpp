#include "numbers.h"
#include "ui_numbers.h"
#include <QMouseEvent>

Numbers::Numbers(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Numbers)
{
    ui->setupUi(this);
}

void Numbers::mousePressEvent(QMouseEvent *event)
{

    int x = event->pos().x();
    int y = event->pos().y();
    for(int i = 0; i < 4; i++){
            QLabel *currLabel = numbers[i];
            int width = currLabel->width();
           // qInfo() << "label width: " << width;
            int height = currLabel->height();
          //  qInfo() << "label height: " << height;
            int labelX = currLabel->x();
          //  qInfo() << "label X POS: " << labelX;
            int labelY = currLabel->y();
          //  qInfo() << "label Y POS: " << labelY;

            if((x >= labelX) && (x <= labelX + width) && (y >= labelY) && (y <= labelY + height)){
               // qInfo() << "Reached here " ;
                QString num = currLabel->text();
               //  qInfo() << "Sending Numer: " << num << " to main window";
                emit(sendNumberClicked(num));

            }
    }
}

Numbers::~Numbers()
{
    delete ui;
}

void Numbers::receiveInitNumbers(int level_)
{
    int x = 0;
    int y = 90;
    level = level_;
    for(int i = 0; i < level; i++){

        if(level == 4){
            QLabel *numberLabel = new QLabel(this);
            numberLabel->setFrameStyle(QFrame::Box);
            QFont font("Arial", 20);
            numberLabel->setStyleSheet("QLabel {border-radius: 0px; border: 1px solid black}");
            numberLabel->setGeometry(x, y, 120, 120);
            numberLabel->setFont(font);
            numberLabel->setAlignment(Qt::AlignCenter);
            numberLabel->setText(QString::number(i+1));
            numbers.push_back(numberLabel);
            if(i % 2 == 0) //even
                x = 130;
            else{ // odd
                x = 0;
                y += 130;
            }
        }
        else if(level == 9){
            QLabel *numberLabel = new QLabel(this);
            numberLabel->setFrameStyle(QFrame::Box);
           // numberLayout->addWidget(numberLabel, 0, i);
            QFont font("Arial", 20);
            numberLabel->setStyleSheet("QLabel {border-radius: 0px; border: 1px solid black}");
            numberLabel->setGeometry(x, y, 80, 80);

            numberLabel->setFont(font);
            numberLabel->setAlignment(Qt::AlignCenter);
            numberLabel->setText(QString::number(i+1));
            numbers.push_back(numberLabel);
            if(i == 2 || i == 5 || i == 8){
                 x = 0;
                 y += 85;
               }
            else // odd
                 x += 85;
        }
    }
    emit(sendNumbers(numbers));
}

