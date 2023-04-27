#include "game.h"
#include "ui_game.h"
#include "scenewidget.h"

Game::Game(Model& model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);

   isExampleMode = false;
   isNoteMode = false;
   prefixVector = model.prefixVector;

   // box2D->hide();
    // initialize index with -1
    indexI = -1;
    indexJ = -1;
    mistakes = 0;
    ui->box2DWidget->hide();
    /* Board Widget */
    board = new Board(ui->boardWidget);
    connect(this, &Game::sendInitBoard, board, &Board::receiveBoard);
    connect(board, &Board::sendCells, this, &Game::receiveCells);


    //wjw
    connect(board, &Board::sendBoxSelected, &model, &Model::get7Vectors);
    connect(&model, &Model::sendVectorsAndIndex, this, &Game::receiveBoxSelected);
    //wjw

    //connect(board, &Board::sendBoxSelected, this, &Game::receiveBoxSelected);
    connect(this, &Game::sendSetVector, board, &Board::receiveVector);

    /* Numbers Widget */
    numbersWidget = new Numbers(ui->numbersWidget);
    connect(this, &Game::sendInitNumbers, numbersWidget, &Numbers::receiveInitNumbers);
    connect(numbersWidget, &Numbers::sendNumberClicked, this, &Game::receiveNumberClicked);
    connect(numbersWidget, &Numbers::sendNumbers, this, &Game::receiveNumbers);
    numbersWidget->setVisible(true);

    /* Game Won Widget */
    gameWon = new gamewondialog(ui->gameWonWidget);
    gameWon->setVisible(false);
    ui->gameWonWidget->setVisible(false);

    /* Game Over Widget */
    gameOver = new GameOverDialog(ui->gameOverDialog);
    connect(gameOver, &GameOverDialog::sendSecondChance, this, &Game::receiveSecondChace);
    connect(gameOver, &GameOverDialog::sendNewGame, this, &Game::receiveNewGame);
    connect(gameOver, &GameOverDialog::sendTutorial, this, &Game::receiveTutorial);

    level = 0;

    /* Erase Button */
    ui->eraseButton->setIcon(QIcon(":/images/eraser.png"));
    ui->eraseButton->setIconSize(QSize(60,60));
    ui->eraseButton->setStyleSheet("QPushButton {border-radius: 10px; border: 1px solid black;}");
    connect(ui->eraseButton, &QPushButton::clicked, this, &Game::eraseButtonClicked);
    connect(this, &Game::sendErase, &model, &Model::receiveErase);
    //connect(this, &MainWindow::reverseRedBoard, board, &Board::setBackColor);

    /* Undo Button */
    ui->undoButton->setIcon(QIcon(":/images/undo.png"));
    ui->undoButton->setIconSize(QSize(60,60));
    ui->undoButton->setStyleSheet("QPushButton {border-radius: 10px; border: 1px solid black;}");
   // connect(this, &MainWindow::sendUndo, &model, &Model::receiveErase);

    /* Notes Button */
    ui->notesButton->setIcon(QIcon(":/images/notes.png"));
    ui->notesButton->setIconSize(QSize(60,60));
    ui->notesButton->setStyleSheet("QPushButton {border-radius: 10px; border: 1px solid black;}");
   // connect(this, &MainWindow::sendUndo, &model, &Model::receiveErase);


    /* Model */
    connect(this, &Game::sendPuzzleInput, &model, &Model::receivePuzzleInput);



    connect(&model, &Model::sendCorrectInput, this, &Game::receiveCorrectInput);
    connect(&model, &Model::sendIncorrectInput, this, &Game::receiveIncorrectInput);
    connect(&model, &Model::sendWonGame, this, &Game::receiveWonGame);
    connect(&model, &Model::sendDispplayVector, this, &Game::receiveDisplayVector);
    /* Send Init Model */
     connect(this, &Game::sendInitModel, &model, &Model::receiveInitModel);
     connect(this, &Game::sendInitExampleModel, &model, &Model::receiveInitExampleModel);

}

Game::~Game()
{
    delete ui;
}

//wjw
void Game::receiveBoxSelected(int j, int i,
                              std::vector<int> numRow, std::vector<int> possibleNumRow,
                              std::vector<int> numCol, std::vector<int> possibleNumCol,
                              std::vector<int> numBox, std::vector<int> possibleNumBox,
                              std::vector<int> intersectNum, bool isFixedNum)
{
    indexJ = j;
    indexI = i;

//    for(int i=0;i<level;i++){
//        for(int j=0;j<level;j++){
//            cells[i][j]->
//        }
//    }

    for(int i=0;i<4;i++)
        numbers[i]->setStyleSheet("QLabel {border-radius: 0px; border: 1px solid black}");

    QString hint = "For the selecting cell,\n\nwe already have ";
    for(int i = 0; i < numRow.size(); i++){
        hint += QString::number(numRow[i]);
        if (i != numRow.size() - 1) {
            hint += ",";
        }
    }
    hint += " in row,\n";
    hint += "so we can only put ";
    for(int i = 0; i < possibleNumRow.size(); i++){
        hint += QString::number(possibleNumRow[i]);
        if (i != possibleNumRow.size() - 1) {
            hint += ",";
        }
    }
    hint += " in this row\n\n";

    // check col
    hint += "we already have ";
    for(int i = 0; i < numCol.size(); i++){
        hint += QString::number(numCol[i]);
        if (i != numCol.size() - 1) {
            hint += ",";
        }
    }
    hint += " in column,\n";
    hint += "so we can only put ";
    for(int i = 0; i < possibleNumCol.size(); i++){
        hint += QString::number(possibleNumCol[i]);
        if (i != possibleNumCol.size() - 1) {
            hint += ",";
        }
    }
    hint += " in this column\n\n";

    //check box
    hint += "we already have ";
    for(int i = 0; i < numBox.size(); i++){
        hint += QString::number(numBox[i]);
        if (i != numBox.size() - 1) {
            hint += ",";
        }
    }
    hint += " in 2*2 square,\n";
    hint += "so we can only put ";
    for(int i = 0; i < possibleNumBox.size(); i++){
        hint += QString::number(possibleNumBox[i]);
        if (i != possibleNumBox.size() - 1) {
            hint += ",";
        }
    }
    hint += " in this 2*2 square\n\n";

    //give possible answer
    hint += "Combine all the information above, \n";
    hint += "(";
    for (int i = 0; i < possibleNumRow.size(); ++i) {
        hint += QString::number(possibleNumRow[i]);
        if (i != possibleNumRow.size() - 1) {
            hint += ",";
        }
    }
    hint += ")∩(";

    for (int i = 0; i < possibleNumCol.size(); ++i) {
        hint += QString::number(possibleNumCol[i]);
        if (i != possibleNumCol.size() - 1) {
            hint += ",";
        }
    }
    hint += ")∩(";

    for (int i = 0; i < possibleNumBox.size(); ++i) {
        hint += QString::number(possibleNumBox[i]);
        if (i != possibleNumBox.size() - 1) {
            hint += ",";
        }
    }
    hint += ")\n";
    hint += "the possible answer for this cell is ";
    if(intersectNum.size() == 0){
        hint += "\n\nNo possible answer for this cell,\n";
        hint += "check the wrong number filled in other cell";

        if(isExampleMode)
            cells[indexJ][indexI]->setEnabled(false);
    }else{
        for (int i = 0; i < intersectNum.size(); ++i) {
            hint += QString::number(intersectNum[i]);
            if (i != intersectNum.size() - 1) {
                hint += ",";
            }
        }
    }

    if(intersectNum.size() > 1){
        hint += "\n\nThis cell have " +
                QString::number(intersectNum.size()) +
                " possible answer,\n" +
                "please choose other cell to fill in first";
        QPalette palette = cells[indexJ][indexI]->palette();
        palette.setColor(QPalette::Disabled, QPalette::Base, Qt::white); // set transparent for disabled background
        palette.setColor(QPalette::Disabled, QPalette::WindowText, Qt::black); // set black for disabled text
        cells[indexJ][indexI]->setPalette(palette);
        if(isExampleMode)
            cells[indexJ][indexI]->setEnabled(false);
    }else if(intersectNum.size() == 1){
        "\n\nThe correct answer for this cell is " + QString::number(intersectNum[0]);
        numbersDisable.clear();
        for(int i=0;i<4;i++){
            if(numbers[i]->text() != QString::number(intersectNum[0])){
                numbersDisable.push_back(i+1);
            }else{
                if(isExampleMode)
                    numbers[i]->setStyleSheet("QLabel {  background-color: green;}");
            }
        }
        cells[indexJ][indexI]->setEnabled(true);
    }

    if(isFixedNum){
        ui->label->setText("");
    }else{
        ui->label->setText(hint);
    }

}

void Game::receiveNumbers(QVector<QLabel *> nums)
{
    numbers = nums;
}

void Game::receiveNumberClicked(QString num)
{
    if(isNoteMode && vector[indexJ][indexI] == 0 && prefixVector[indexJ][indexI] == 0){
       QString text = cells[indexJ][indexI]->text();
       QString str1 = "";
       if(text.contains("1"))
           str1 = "1";
       QString str2 = "";
       if(text.contains("2"))
           str2 = "2";
       QString str3 = "";
       if(text.contains("3"))
           str3 = "3";
       QString str4 = "";
       if(text.contains("4"))
           str4 = "4";
       QString empty = "\n";

       if(!text.contains("1") && num == "1")
            str1 = "1";
       else if(text.contains("1") && num == "1")
            str1 = "";

       if(!text.contains("2") && num == "2")
            str2 = "2";
       else if(text.contains("2") && num == "2")
            str2 = "";

       if(!text.contains("3") && num == "3")
            str3 = "3";
       else if(text.contains("3") && num == "3")
            str3 = "";

       if(!text.contains("4") && num == "4")
            str4 = "4";
       else if(text.contains("4") && num == "4")
            str4 = "";

       cells[indexJ][indexI]->setText("<font color='red'>" + str1 + empty + "</font><font color='blue'>" + str2 + "</font>" +  "</font><br><font color='green'>" + str3 + empty + "</font><font color='purple'>" + str4 + "</font>");
    }
    else if (!isNoteMode){
        if((indexI != -1 && indexJ != -1) && cells[indexJ][indexI]->isEnabled()){
            if(isExampleMode){
                auto it = std::find(numbersDisable.begin(), numbersDisable.end(), num.toInt());
                if(it == numbersDisable.end()){
                    cells[indexJ][indexI]->setText(num);
                    emit sendPuzzleInput(num.toInt(), indexJ, indexI, level,false);
                    qDebug() << "receiveNumberClicked is hit";
                    for(int i=0;i<4;i++)
                        numbers[i]->setStyleSheet("QLabel {border-radius: 0px; border: 1px solid black}");
                }
            }else{
                cells[indexJ][indexI]->setText(num);
                emit sendPuzzleInput(num.toInt(), indexJ, indexI, level,false);
                qDebug() << "receiveNumberClicked is hit";
            }
        }
    }

}

void Game::receiveCorrectInput(int input, int indexJ, int indexI)
{
    //background-color is light blue which highlight the current box, and set the correct number to green
    cells[indexJ][indexI]->setStyleSheet("QLabel {background-color : rgba(173, 216, 230, 128); color: rgba(0, 165, 11, 1);}");
    cells[indexJ][indexI]->setText(numbers[input - 1]->text());
    vector[indexJ][indexI] = 1;
    emit sendSetVector(indexJ, indexI, 1);
}

void Game::receiveIncorrectInput(int input, int indexJ, int indexI)
{
    mistakes++;
    cells[indexJ][indexI]->setStyleSheet("QLabel {background-color : rgba(173, 216, 230, 128); color: red;}");
    cells[indexJ][indexI]->setText(numbers[input - 1]->text());
    vector[indexJ][indexI] = 2;
    emit sendSetVector(indexJ,indexI,2);
    ui->mistakesLabel->setText(QString("Mistakes: %1/5").arg(mistakes));
    if(mistakes == 5){

        gameOver->setVisible(true);
    }
}

/**
 * @brief MainWindow::shonkWin
 * When win the game, show congratulation message
 */
void Game::receiveWonGame()
{
    box2D->show();
}

void Game::receiveSecondChace()
{
    mistakes--;
    ui->mistakesLabel->setText(QString("Mistakes: %1/5").arg(mistakes));
    gameOver->setVisible(false);

}

void Game::receiveNewGame()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void Game::receiveTutorial()
{

}

void Game::receiveCells(QVector<QVector<QLabel *>> cells_)
{
    cells = cells_;
    board->setVisible(true);
}

/**
 * @brief MainWindow::on_eraseButton_clicked
 * When click the erase button, remove the current selected number.
 * If nothing has been selected, click this button won't work
 */
void Game::eraseButtonClicked()
{

    gameWon->setVisible(true);
//    ui->gameWonWidget->setVisible(true);
//    ui->box2DWidget->show();
    qDebug() << "cells[indexJ][indexI]->isEnabled()";

    //qDebug() << "cells[indexJ][indexI]->isEnabled()";
    cells[indexJ][indexI]->setEnabled(true);

    if(indexI != -1 && indexJ != -1)
    {
        qDebug() << cells[indexJ][indexI]->isEnabled();
        if(cells[indexJ][indexI]->isEnabled())
        {

            emit sendErase(indexJ, indexI);

            cells[indexJ][indexI]->setText("");
            vector[indexJ][indexI] = 0;
            ui->eraseButton->setStyleSheet("QPushButton {border-radius: 10px; border: 1px solid black;background-color : rgba(173, 216, 230, 128);}");
            QTimer::singleShot(100, this, &Game::eraseButtonDisplay);
            sendSetVector(indexJ,indexI,0);
        }
    }
}

void Game::on_backButton_clicked()
{
    this->hide();
    emit goToMenuPage();
}

void Game::receiveLevel4(int level_)
{
    isExampleMode = false;
    level = level_;
    vector =  std::vector<std::vector<int>>(level, std::vector<int>(level, 0));
    emit(sendInitModel(4));
    this->show();
    ui->label->hide();
}

void Game::receiveLevel9(int level_)
{
    isExampleMode = false;
    level = level_;
    vector =  std::vector<std::vector<int>>(level, std::vector<int>(level, 0));
    emit(sendInitModel(9));
    this->show();
    ui->label->hide();
}

void Game::receiveLevelExample(int level_)
{
    isExampleMode = true;
    level = level_;
    vector =  std::vector<std::vector<int>>(level, std::vector<int>(level, 0));
    emit(sendInitExampleModel(4));
    this->show();
    ui->label->show();
}

void Game::receiveDisplayVector(std::vector<std::vector<int>> dv)
{
    prefixVector = dv;
    emit(sendInitBoard(level, dv));
    emit(sendInitNumbers(level));
}

void Game::eraseButtonDisplay()
{
    ui->eraseButton->setStyleSheet("QPushButton {border-radius: 10px; border: 1px solid black;background-color : rgba(0, 0, 0, 0);}");
}


void Game::on_notesButton_clicked()
{
    isNoteMode = !isNoteMode;
    if(isNoteMode){
        ui->notesButton->setStyleSheet("QPushButton {border-radius: 10px; border: 1px solid black;background-color : rgba(173, 216, 230, 128);}");
    }
    else{
        ui->notesButton->setStyleSheet("QPushButton {border-radius: 10px; border: 1px solid black;background-color : rgba(0, 0, 0, 0);}");
    }
}

