#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"
#include "tutorial.h"
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model& model, QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void initializeTutorial();
    void sendEasyButton(int);
    void sendHardButton(int);
    void sendExampleButton(int);

private slots:
    void showMenuPage();
    void on_tutorialButton_clicked();
    void on_exitButton_clicked();
    void on_easyButton_clicked();
    void on_hardButton_clicked();
    void onExampleButtonClicked();

private:
    Ui::MainWindow *ui;
    Tutorial tutorialWindow;
    Game* gameWindow;
};
#endif // MAINWINDOW_H
