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


    std::vector<int> generateNumVector(int);
    std::vector<int> findSameNumsInVectors(std::vector<int>, std::vector<int>, std::vector<int>);
    std::vector<int> sameNumInBox4(int, int);
    std::vector<int> sameNumInBox9(int, int);


signals:
    void sendCorrectInput(int, int, int);
    void sendIncorrectInput(int, int, int);
    void sendWonGame();
    void sendDispplayVector(std::vector<std::vector<int>>, std::vector<std::vector<int>>);

    //wjw
    void sendVectorsAndIndex(int, int,
                             std::vector<int>, std::vector<int>,
                             std::vector<int>, std::vector<int>,
                             std::vector<int>, std::vector<int>,
                             std::vector<int>, bool);

public slots:
    void changeLevel(int level);
    void generator();

    //wjw
    void receivePuzzleInput(int, int, int, int, bool);
    //wjw

    void receiveErase(int, int);
    void receiveInitModel(int);
    void receiveInitExampleModel(int);

    //wjw
    void get7Vectors(int, int, int);

};

#endif // MODEL_H
