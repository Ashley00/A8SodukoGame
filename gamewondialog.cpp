#include "gamewondialog.h"
#include "ui_gamewondialog.h"
#include <QProcess>

gamewondialog::gamewondialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gamewondialog)
{
    ui->setupUi(this);
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
