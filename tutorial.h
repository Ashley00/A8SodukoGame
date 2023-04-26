#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QString>

namespace Ui {
class Tutorial;
}

class Tutorial : public QWidget
{
    Q_OBJECT

public:
    explicit Tutorial(QWidget *parent = nullptr);
    ~Tutorial();
    int currentSlide;

signals:
    void goToMenuPage();
    void sendExampleButtonClicked();

public slots:
    void initializeTutorial();
    void on_backButton_clicked();
    void on_rightButton_clicked();
    void on_leftButton_clicked();
    void onExampleButtonClicked();

private:
    Ui::Tutorial *ui;
    QVector<QPixmap> slides;

    int totalSlides = 29;

};

#endif // TUTORIAL_H
