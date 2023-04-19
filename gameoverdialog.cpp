#include "gameoverdialog.h"
#include "ui_gameoverdialog.h"

GameOverDialog::GameOverDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameOverDialog)
{
    ui->setupUi(this);
    connect(ui->secondChanceBtn, &QPushButton::clicked, this, &GameOverDialog::onSecondChanceClicked);
    connect(ui->newGameBtn, &QPushButton::clicked, this, &GameOverDialog::onNewGameClicked);
    connect(ui->tutorialBtn, &QPushButton::clicked, this, &GameOverDialog::onTutorialClicked);
}

GameOverDialog::~GameOverDialog()
{
    delete ui;
}
void GameOverDialog::onSecondChanceClicked()
{
    emit(sendSecondChance());
}

void GameOverDialog::onNewGameClicked()
{
    emit(sendNewGame());
}

void GameOverDialog::onTutorialClicked()
{
    emit(sendTutorial());
}

