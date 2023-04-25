#ifndef GAMEOVERDIALOG_H
#define GAMEOVERDIALOG_H

#include <QDialog>

namespace Ui {
class GameOverDialog;
}

class GameOverDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameOverDialog(QWidget *parent = nullptr);
    ~GameOverDialog();

signals:
    void sendSecondChance();
    void sendNewGame();
    void sendTutorial();


private slots:
    void onSecondChanceClicked();
    void onNewGameClicked();
    //void onTutorialClicked();

private:
    Ui::GameOverDialog *ui;
};

#endif // GAMEOVERDIALOG_H
