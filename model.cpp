#include "model.h"

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
    solutionVector = {{2,1,4,3},
                      {4,3,2,1},
                      {3,2,1,4},
                      {1,4,3,2}};

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
