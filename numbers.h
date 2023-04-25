#ifndef NUMBERS_H
#define NUMBERS_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>

namespace Ui {
class Numbers;
}

class Numbers : public QWidget
{
    Q_OBJECT

public:
    explicit Numbers(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *);
    ~Numbers();

public slots:
    void receiveInitNumbers(int);

signals:
    void sendNumberClicked(QString);
    void sendNumbers( QVector<QLabel* > );

private:
    Ui::Numbers *ui;
    QVector<QLabel* > numbers;
    int level;
};

#endif // NUMBERS_H
