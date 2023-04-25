#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow Constructor
 * @param parent
 */
MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gameWindow = new Game(model);

    // set menu page background
    QPixmap background(":/images/background.png");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setPalette(palette);

    // set menu page buttons
    ui->tutorialButton->setIcon(QIcon(":/images/tutorial.png"));
    ui->tutorialButton->setIconSize(QSize(160,80));
    ui->easyButton->setIcon(QIcon(":/images/easy.png"));
    ui->easyButton->setIconSize(QSize(160,80));
    ui->mediumButton->setIcon(QIcon(":/images/medium.png"));
    ui->mediumButton->setIconSize(QSize(160,80));
    ui->hardButton->setIcon(QIcon(":/images/hard.png"));
    ui->hardButton->setIconSize(QSize(160,80));
    ui->exitButton->setIcon(QIcon(":/images/exit.png"));
    ui->exitButton->setIconSize(QSize(160,80));

    // signal slot connections
    connect(this, MainWindow::initializeTutorial, &tutorialWindow, Tutorial::initializeTutorial);
    connect(&tutorialWindow, Tutorial::goToMenuPage, this, MainWindow::showMenuPage);
    connect(gameWindow, Game::goToMenuPage, this, MainWindow::showMenuPage);

}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_tutorialButton_clicked
 * Go to tutorial page
 */
void MainWindow::on_tutorialButton_clicked()
{
    this->hide();
    tutorialWindow.show();
    emit initializeTutorial();
}

/**
 * @brief MainWindow::on_exitButton_clicked
 * Exit the game
 */
void MainWindow::on_exitButton_clicked()
{
    qApp->quit();
}

/**
 * @brief MainWindow::showMenuPage
 * Slot to show menu oage
 */
void MainWindow::showMenuPage()
{
    this->show();
}

/**
 * @brief MainWindow::on_easyButton_clicked
 * Go to easy mode
 */
void MainWindow::on_easyButton_clicked()
{
    this->hide();
    gameWindow->show();
}

