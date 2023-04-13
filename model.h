#ifndef MODEL_H
#define MODEL_H

#include <QObject>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    // game level, can be 4*4, 6*6, 9*9
    int level;
    // vector of numbers to display
    std::vector<std::vector<int>> displayVector;
    // vector of the solution
    std::vector<std::vector<int>> solutionVector;

signals:


public slots:
    void changeLevel(int level);
    void generator();

};

#endif // MODEL_H
