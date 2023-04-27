#include "model.h"
#include <QDebug>
#include <algorithm>

/**
 * @brief Model::Model Constructor
 * @param parent
 */
Model::Model(QObject *parent)
    : QObject{parent}
{

}

//wjw
void Model::receivePuzzleInput(int input, int indexJ, int indexI, int level, bool checkall)
{
    std::vector<int> numInRow;
    std::vector<int> numInCol;
    std::vector<int> numInBox4;
    std::vector<int> numInBox9;

    std::vector<int> possibleNumInRow = generateNumVector(level);
    std::vector<int> possibleNumInCol = generateNumVector(level);
    std::vector<int> possibleNumInBox = generateNumVector(level);

    //loop through indexJ row
    for(int c = 0; c < level; c++){
        if(currentVector[indexJ][c] != 0 && c != indexI){
            numInRow.push_back(currentVector[indexJ][c]);
            possibleNumInRow.erase(std::remove(
                                        possibleNumInRow.begin(),
                                        possibleNumInRow.end(),
                                        currentVector[indexJ][c]),
                                        possibleNumInRow.end());
        }
    }
    //loop through indexI col
    for(int r = 0; r < level; r++){
        if(currentVector[r][indexI] != 0 && r != indexJ){
            numInCol.push_back(currentVector[r][indexI]);
            possibleNumInCol.erase(std::remove(
                                        possibleNumInCol.begin(),
                                        possibleNumInCol.end(),
                                        currentVector[r][indexI]),
                                        possibleNumInCol.end());
        }
    }
    //loop through the box that contains selected index
    if(level == 4){
        numInBox4 = sameNumInBox4(indexJ, indexI);
        for(int i = 0; i < numInBox4.size(); i++){
            possibleNumInBox.erase(std::remove(
                                        possibleNumInBox.begin(),
                                        possibleNumInBox.end(),
                                        numInBox4.at(i)),
                                        possibleNumInBox.end());
        }
    }else if(level == 9){
        numInBox9 = sameNumInBox9(indexJ, indexI);
        for(int i = 0; i < numInBox9.size(); i++){
            possibleNumInBox.erase(std::remove(
                                        possibleNumInBox.begin(),
                                        possibleNumInBox.end(),
                                        numInBox9.at(i)),
                                        possibleNumInBox.end());
        }
    }

    std::vector commonNumVec = findSameNumsInVectors(possibleNumInRow, possibleNumInCol, possibleNumInBox);

    bool inputIsPossible = std::find(commonNumVec.begin(), commonNumVec.end(), input) != commonNumVec.end();

    if(!checkall)
        currentVector[indexJ][indexI] = input;

    if(inputIsPossible){
        //currentVector[indexJ][indexI] = input;
        if(currentVector == solutionVector){
            emit sendCorrectInput(input, indexJ, indexI);
            emit sendWonGame();
        }
        emit sendCorrectInput(input, indexJ, indexI);
    }else{
        emit sendIncorrectInput(input, indexJ, indexI);
    }
}

//wjw
void Model::get7Vectors(int indexJ, int indexI, int _level){
    std::vector<int> numInRow;
    std::vector<int> numInCol;
    std::vector<int> numInBox4;

    std::vector<int> possibleNumInRow = generateNumVector(level);
    std::vector<int> possibleNumInCol = generateNumVector(level);
    std::vector<int> possibleNumInBox = generateNumVector(level);

    //loop through indexJ row
    for(int c = 0; c < level; c++){
        if(currentVector[indexJ][c] != 0 && c != indexI){
            numInRow.push_back(currentVector[indexJ][c]);
            possibleNumInRow.erase(std::remove(
                                        possibleNumInRow.begin(),
                                        possibleNumInRow.end(),
                                        currentVector[indexJ][c]),
                                        possibleNumInRow.end());
        }
    }
    //loop through indexI col
    for(int r = 0; r < level; r++){
        if(currentVector[r][indexI] != 0 && r != indexJ){
            numInCol.push_back(currentVector[r][indexI]);
            possibleNumInCol.erase(std::remove(
                                        possibleNumInCol.begin(),
                                        possibleNumInCol.end(),
                                        currentVector[r][indexI]),
                                        possibleNumInCol.end());
        }
    }
    //loop through the box that contains selected index
    if(_level == 4){
        numInBox4 = sameNumInBox4(indexJ, indexI);
        for(int i = 0; i < numInBox4.size(); i++){
            possibleNumInBox.erase(std::remove(
                                        possibleNumInBox.begin(),
                                        possibleNumInBox.end(),
                                        numInBox4.at(i)),
                                        possibleNumInBox.end());
        }
    }

    std::vector commonNumVec = findSameNumsInVectors(possibleNumInRow, possibleNumInCol, possibleNumInBox);

    bool isFixedNum = false;
    if(displayVector[indexJ][indexI] != 0 || currentVector[indexJ][indexI] != 0){
        isFixedNum = true;
    }

    emit sendVectorsAndIndex(indexJ, indexI,
                             numInRow, possibleNumInRow,
                             numInCol, possibleNumInCol,
                             numInBox4, possibleNumInBox,
                             commonNumVec, isFixedNum);

}

//wjw
/**
 * @brief Model::generateNumVector
 * Generate a vector that contains 1 to given number
 * @param totalNum
 * @return
 */
std::vector<int> Model::generateNumVector(int totalNum) {
    std::vector<int> vec;
      for (int i = 1; i <= totalNum; i++) {
        vec.push_back(i);
      }
      return vec;
  }

//wjw
/**
 * @brief Model::findSameNumsInVectors
 * Helper method that finds common numbers in three vectors
 * @param v1
 * @param v2
 * @return
 */
std::vector<int> Model::findSameNumsInVectors(std::vector<int> v1, std::vector<int> v2, std::vector<int> v3){
    std::vector<int> tempVec;
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    std::sort(v3.begin(), v3.end());
    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(tempVec));
    std::vector<int> sameNumVector;
    std::set_intersection(tempVec.begin(), tempVec.end(), v3.begin(), v3.end(), std::back_inserter(sameNumVector));
    return sameNumVector;
}

//wjw
/**
 * @brief Model::sameNumInBox4
 * In 4*4 sudoku, get all filled numbers in 2*2 box that contains selected cell
 * @param r
 * @param c
 * @return
 */
std::vector<int> Model::sameNumInBox4(int r, int c){
    std::vector<int> resultVec;
    if((r>=0 && r<2) && (c>=0 && c<2)){
        for(int x = 0; x < 2; x++){
            for(int y = 0; y < 2; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }else if((r>=0 && r<2) && (c>=2 && c<4)){
        for(int x = 0; x < 2; x++){
            for(int y = 2; y < 4; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }else if((r>=2 && r<4) && (c>=0 && c<2)){
        for(int x = 2; x < 4; x++){
            for(int y = 0; y < 2; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }else{
        for(int x = 2; x < 4; x++){
            for(int y = 2; y < 4; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }
    return resultVec;
}

//wjw
/**
 * @brief Model::sameNumInBox9
 * In 9*9 sudoku, get all filled numbers in 3*3 box that contains selected cell
 * @param r
 * @param c
 * @return
 */
std::vector<int> Model::sameNumInBox9(int r, int c){
    std::vector<int> resultVec;
    if((r>=0 && r<3) && (c>=0 && c<3)){
        for(int x=0; x<3; x++){
            for(int y=0; y<3; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }else if((r>=0 && r<3) && (c>=3 && c<6)){
        for(int x=0; x<3; x++){
            for(int y=3; y<6; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }else if((r>=0 && r<3) && (c>=6 && c<9)){
        for(int x=0; x<3; x++){
            for(int y=6; y<9; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }else if((r>=3 && r<6) && (c>=0 && c<3)){
        for(int x=3; x<6; x++){
            for(int y=0; y<3; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }else if((r>=3 && r<6) && (c>=3 && c<6)){
        for(int x=3; x<6; x++){
            for(int y=3; y<6; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }else if((r>=3 && r<6) && (c>=6 && c<9)){
        for(int x=3; x<6; x++){
            for(int y=6; y<9; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }else if((r>=6 && r<9) && (c>=0 && c<3)){
        for(int x=6; x<9; x++){
            for(int y=0; y<3; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }else if((r>=6 && r<9) && (c>=3 && c<6)){
        for(int x=6; x<9; x++){
            for(int y=3; y<6; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }else{
        for(int x=6; x<9; x++){
            for(int y=6; y<9; y++){
                if(currentVector[x][y] != 0 && !(x == r && y == c)){
                    resultVec.push_back(currentVector[x][y]);
                }
            }
        }
    }
    return resultVec;
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

void Model::receiveInitModel(int level_)
{
    // initialize with 4*4 Sudoku game
    level = level_;
    generateGame(level);
    emit(sendDispplayVector(displayVector, solutionVector));
}

void Model::receiveInitExampleModel(int level_)
{
    // initialize with 4*4 Sudoku game
    level = level_;
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
    emit(sendDispplayVector(displayVector, solutionVector));
}

