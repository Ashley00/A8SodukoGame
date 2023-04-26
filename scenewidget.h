#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>
#include <Box2D/Box2D.h>
#include <QTimer>

class SceneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SceneWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void updateWorld();
  //  void triggerBox2D();

private:
    b2World world;
    b2Body* body;
    b2Body* body1;
    b2Body* body2;
    QVector<b2Body*> bodies;
    QTimer timer;
    QImage image;
    QVector<QRect> boxes;
    QRect box;
    double x;
    double y;
};

#endif // SCENEWIDGET_H
