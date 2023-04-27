#include "scenewidget.h"
#include <QPainter>
#include <QDebug>

SceneWidget::SceneWidget(QWidget *parent) : QWidget(parent),
    world(b2Vec2(0.0f, 10.0f)),
    timer(this),
    //image(":/sprites/alien1.png")
    image(":/images/star.png")
{
    image = image.scaled(50, 50, Qt::KeepAspectRatio);

    // ******** body 0 ********* //
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 20.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, .0f);

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(5.0f, -4.0f); // this one

    body = world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 5.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.9;
    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);
    //printf("Init world\n");


    connect(&timer, &QTimer::timeout, this, &SceneWidget::updateWorld);


    timer.start(10);

}

void SceneWidget::paintEvent(QPaintEvent *) {
    // Create a painter
    QPainter painter(this);
    b2Vec2 position = body->GetPosition();
  //  float angle = body->GetAngle();
    QColor clr(255, 194, 226, 255);
    QBrush brush(clr);
//    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
   // painter.setBrush(brush);
    painter.drawRect(-10,-10, 1000, 1000);

         painter.drawImage((int)(position.x), (int)(position.y*95), image);
         painter.drawImage((int)(position.x), (int)(position.y*55), image);
         painter.drawImage((int)(position.x + 100), (int)(position.y*100), image);
         painter.drawImage((int)(position.x + 100), (int)(position.y*150), image);
         painter.drawImage((int)(position.x + 200), (int)(position.y*90), image);
         painter.drawImage((int)(position.x + 200), (int)(position.y*40), image);
         painter.drawImage((int)(position.x + 300), (int)(position.y*105), image);
         painter.drawImage((int)(position.x + 300), (int)(position.y*170), image);
         painter.drawImage((int)(position.x + 400), (int)(position.y*80), image);
         painter.drawImage((int)(position.x + 400), (int)(position.y*140), image);
         painter.drawImage((int)(position.x + 500), (int)(position.y*110), image);
         painter.drawImage((int)(position.x + 500), (int)(position.y*50), image);
         painter.drawImage((int)(position.x + 600), (int)(position.y*100), image);
         painter.drawImage((int)(position.x + 600), (int)(position.y*170), image);
         painter.drawImage((int)(position.x + 700), (int)(position.y*97), image);
         painter.drawImage((int)(position.x + 700), (int)(position.y*31), image);


//         painter.drawImage((int)(position.x + 100), (int)(position.y*101), image);
//         painter.drawImage((int)(position.x + 200), (int)(position.y*102), image);
//         painter.drawImage((int)(position.x + 300), (int)(position.y*103), image);
//         painter.drawImage((int)(position.x + 400), (int)(position.y*104), image);

         painter.end();
}

void SceneWidget::recreateBody()
{
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(5.0f, -4.0f); // this one

    body = world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 5.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.9;
    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);
}

void SceneWidget::updateWorld() {
    // It is generally best to keep the time step and iterations fixed.
    world.Step(1.0/120.0, 6, 2);
    update();
}
