#include "gameplaymechanics.h"
#include <QRandomGenerator>

//Create the rules and gameplay of tetris

GamePlayMechanics::GamePlayMechanics()
{
    //Set up 2D array of squares of tetris
    bool gameField[6][9] = {};
}


int GamePlayMechanics::GetTetromino()
{
    //Select a random block out of the seven tetrominos. Discard if equal to last one.
    int oldTetromino, currentTetromino;

    while (oldTetromino == currentTetromino)
    {
        currentTetromino = QRandomGenerator::global()->bounded(0, 7);
        qDebug() << currentTetromino;
    }
    oldTetromino = currentTetromino;

    qDebug() << oldTetromino << currentTetromino;

    return currentTetromino;
}

void GamePlayMechanics::ActiveTetromino(int tetromino)
{

}

void GamePlayMechanics::ManageGameplayLines(bool gameField)
{

}
