#include "model.h"
#include <QDebug>

/**
 * @brief Model::Model Constructor
 * @param parent
 */
Model::Model(QObject *parent)
    : QObject{parent}
{
    // initialize with 4*4 Sudoku game
    level = 4;
    displayVector = {{2,1,0,0},
                     {0,3,2,0},
                     {0,0,0,4},
                     {1,0,0,0}};
    prefixVector = {{2,1,0,0},
                     {0,3,2,0},
                     {0,0,0,4},
                     {1,0,0,0}};
    solutionVector = {{2,1,4,3},
                      {4,3,2,1},
                      {3,2,1,4},
                      {1,4,3,2}};
    currentVector = displayVector;
    
    generateGame(level);
}

void Model::receivePuzzleInput(int input, int indexJ, int indexI)
{
    qDebug() << "User input received...:";
    qDebug() << "Input number: " << input << " indexJ: " << indexJ << " indexI: " << indexI;
     /* Check to see if input is correct */
     if(solutionVector[indexJ][indexI] == input){
         currentVector[indexJ][indexI] = input;
         if(currentVector == solutionVector){
             emit sendCorrectInput(input, indexJ, indexI);
             emit sendWonGame();
         }else{
             emit sendCorrectInput(input, indexJ, indexI);
         }
     }else{
         emit sendIncorrectInput(input, indexJ, indexI);
     }
}


/**
 * @brief Model::changeLevel
 * Change the sudoku game level, level can be in 4*4, 6*6, 9*9
 * @param level
 */
void Model::changeLevel(int level)
{
    level = level;

}

/**
 * @brief Model::generator
 * Sudoku game algorithm random generator with answers for 9*9 game
 */
void Model::generator()
{
    // resize both display and solution vectors to be 9*9
    int gameSize = 9;
    displayVector.resize(gameSize);
    solutionVector.resize(gameSize);
    for(int i = 0; i < gameSize; i++)
    {
        displayVector[i].resize(gameSize);
        solutionVector[i].resize(gameSize);
    }

    for(int i = 0; i < gameSize; i++)
    {
        displayVector[i] = {0,0,0,0,0,0,0,0,0};
        solutionVector[i] = {0,0,0,0,0,0,0,0,0};
    }
}

void Model::generateGame(int level_)
{
    level = level_;
    if(level == 4){
        displayVector = {{2,1,0,0},
                         {0,3,2,0},
                         {0,0,0,4},
                         {1,0,0,0}};
        prefixVector = {{2,1,0,0},
                         {0,3,2,0},
                         {0,0,0,4},
                         {1,0,0,0}};
        solutionVector = {{2,1,4,3},
                          {4,3,2,1},
                          {3,2,1,4},
                          {1,4,3,2}};
        currentVector = displayVector;
    }

    if(level == 9){
        displayVector = {{1,2,0,6,0,8,0,0,0},
                         {5,8,4,2,3,9,7,0,1},
                         {0,6,0,1,4,0,0,0,0},
                         {3,7,0,0,6,1,5,8,0},
                         {6,9,1,0,8,0,2,7,4},
                         {4,5,8,7,0,2,0,1,3},
                         {0,3,0,0,2,4,1,5,0},
                         {2,0,9,8,5,0,4,3,6},
                         {0,0,0,3,0,6,0,9,2}
                        };
        prefixVector = {{1,2,0,6,0,8,0,0,0},
                        {5,8,4,2,3,9,7,0,1},
                        {0,6,0,1,4,0,0,0,0},
                        {3,7,0,0,6,1,5,8,0},
                        {6,9,1,0,8,0,2,7,4},
                        {4,5,8,7,0,2,0,1,3},
                        {0,3,0,0,2,4,1,5,0},
                        {2,0,9,8,5,0,4,3,6},
                        {0,0,0,3,0,6,0,9,2}
                       };
        solutionVector = {{1,2,3,6,7,8,9,4,5},
                          {5,8,4,2,3,9,7,6,1},
                          {9,6,7,1,4,5,3,2,8},
                          {3,7,2,4,6,1,5,8,9},
                          {6,9,1,5,8,3,2,7,4},
                          {4,5,8,7,9,2,6,1,3},
                          {8,3,6,9,2,4,1,5,7},
                          {2,1,9,8,5,7,4,3,6},
                          {7,4,5,3,1,6,8,9,2}
                         };
        currentVector = displayVector;
    }
}

/**
 * @brief Model::erase
 * Erase the selected number from displayVector in model
 */
void Model::receiveErase(int indexI, int indexJ)
{
    displayVector[indexI][indexJ] = 0;
    currentVector[indexI][indexJ] = 0;
    //qDebug() << "display" << displayVector[indexI][indexJ] << "current" << currentVector[indexI][indexJ];
}

