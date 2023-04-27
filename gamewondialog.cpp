#include "gamewondialog.h"
#include "ui_gamewondialog.h"
#include <QProcess>

gamewondialog::gamewondialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gamewondialog)
{
    ui->setupUi(this);
    // set menu page background
    QPixmap background(":/images/background.png");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setPalette(palette);

    connect(ui->mainMenuButton, &QPushButton::clicked, this, &gamewondialog::mainMenu);
}

gamewondialog::~gamewondialog()
{
    delete ui;
}

void gamewondialog::mainMenu()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}
