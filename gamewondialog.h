#ifndef GAMEWONDIALOG_H
#define GAMEWONDIALOG_H

#include <QWidget>

namespace Ui {
class gamewondialog;
}

class gamewondialog : public QWidget
{
    Q_OBJECT

public:
    explicit gamewondialog(QWidget *parent = nullptr);
    ~gamewondialog();

public slots:
    void mainMenu();
private:
    Ui::gamewondialog *ui;
};

#endif // GAMEWONDIALOG_H
