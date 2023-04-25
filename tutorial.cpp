#include "tutorial.h"
#include "ui_tutorial.h"

/**
 * @brief Tutorial::Tutorial Constructor
 * @param parent
 */
Tutorial::Tutorial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tutorial)
{
    ui->setupUi(this);

    // set tutorial page background
    QPixmap background(":/images/full.png");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setPalette(palette);

    // add all slides to QVector as QPixmap
    for(int i = 1; i < 5; i++){
        QString imagePath = ":/images/slide" + QString::number(i) + ".png";
        //QImage image = QImage(ui->canvas->size().height(), ui->canvas->size().width(), QImage::Format_RGB32);
        QPixmap pixmap(imagePath);
        QPixmap scaledPixmap = pixmap.scaled(511,361,Qt::KeepAspectRatio);
        slides.push_back(scaledPixmap);
    }

    // show first slide
    ui->leftButton->setEnabled(false);
    currentSlide = 0;
    ui->slidesLabel->setPixmap(slides.at(currentSlide));

}

/**
 * @brief Tutorial::~Tutorial
 */
Tutorial::~Tutorial()
{
    delete ui;
}

/**
 * @brief Tutorial::initializeTutorial
 * Show first slide as initialization each time when click the Tutorial button
 */
void Tutorial::initializeTutorial()
{
    ui->leftButton->setEnabled(false);
    currentSlide = 0;
    ui->slidesLabel->setPixmap(slides.at(currentSlide));
}

/**
 * @brief Tutorial::on_backButton_clicked
 * Go back to Menu page
 */
void Tutorial::on_backButton_clicked()
{
    this->hide();
    emit goToMenuPage();
}

/**
 * @brief Tutorial::on_rightButton_clicked
 * Go to next slide
 */
void Tutorial::on_rightButton_clicked()
{
    currentSlide += 1;
    ui->slidesLabel->setPixmap(slides.at(currentSlide));

    // enbale or disbale arrow button
    if(currentSlide != 0){
        ui->leftButton->setEnabled(true);
    }
    if(currentSlide+1 == totalSlides){
        ui->rightButton->setEnabled(false);
    }
}

/**
 * @brief Tutorial::on_leftButton_clicked
 * Go to previous slide
 */
void Tutorial::on_leftButton_clicked()
{
    currentSlide -= 1;
    ui->slidesLabel->setPixmap(slides.at(currentSlide));

    // enbale or disbale arrow button
    if(currentSlide+1 != totalSlides){
        ui->rightButton->setEnabled(true);
    }
    if(currentSlide == 0){
        ui->leftButton->setEnabled(false);
    }
}

