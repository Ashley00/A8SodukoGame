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

    std::vector<std::vector<int>> prefixVector;

    // vector of the solution
    std::vector<std::vector<int>> solutionVector;
    // vector of the current state
    std::vector<std::vector<int>> currentVector;
    
    void generateGame(int);

signals:
    void sendCorrectInput(int, int, int);
    void sendIncorrectInput(int, int, int);
    void sendWonGame();
    void sendDispplayVector(std::vector<std::vector<int>>);

public slots:
    void changeLevel(int level);
    void generator();
    void receivePuzzleInput(int, int, int);
    void receiveErase(int, int);
    void receiveInitModel(int);

};

#endif // MODEL_H
