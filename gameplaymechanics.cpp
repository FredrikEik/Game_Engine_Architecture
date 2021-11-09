#include "gameplaymechanics.h"
#include <QRandomGenerator>

//Create the rules and gameplay of tetris

GamePlayMechanics::GamePlayMechanics()
{
    //Set up 2D array of squares of tetris
    bool gameField[6][9] = {};
}

GameObject* GamePlayMechanics::TetrominoMaker(int tetromino)
{
    GameObject* GameBlock = {};
    bool tetrominoCraftGrid[4][3] = {{0}};

switch (tetromino)
{
    case 1:
    {
        //Create line
        bool line[4][3] = {{1, 0, 0},
                           {1, 0, 0},
                           {1, 0, 0},
                           {1, 0, 0}};

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
            {
                tetrominoCraftGrid[i][j] = line[i][j];
            }

    break;
    }

    case 2:
    {
        //Create 2x2 block
        bool block[4][3] = {{0, 0, 0},
                            {0, 0, 0},
                            {1, 1, 0},
                            {1, 1, 0}};

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
            {
                tetrominoCraftGrid[i][j] = block[i][j];
            }

    break;
    }

    case 3:
    {
        //Create sigzag
        bool sigzag[4][3] = {{0, 0, 0},
                             {0, 0, 0},
                             {1, 1, 0},
                             {0, 1, 1}};

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
            {
                tetrominoCraftGrid[i][j] = sigzag[i][j];
            }
    break;
    }

    case 4:
    {
        //create reverse sigzag
        bool revSigzag[4][3] = {{0, 0, 0},
                                {0, 0, 0},
                                {0, 1, 1},
                                {1, 1, 0}};

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
            {
                tetrominoCraftGrid[i][j] = revSigzag[i][j];
            }
    break;
    }

    case 5:
    {
        //Create "L" shape
        bool Lshape[4][3] = {{0, 0, 0},
                           {1, 0, 0},
                           {1, 0, 0},
                           {1, 1, 0}};

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
            {
                tetrominoCraftGrid[i][j] = Lshape[i][j];
            }
    break;
    }

    case 6:
    {
        //Create reverse "L" shape
        bool revLshape[4][3] = {{0, 0, 0},
                              {0, 0, 1},
                              {0, 0, 1},
                              {0, 1, 1}};

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
            {
                tetrominoCraftGrid[i][j] = revLshape[i][j];
            }
       break;
    }

    case 7:
    {
        //Create "T" shape
        bool tshape[4][3] = {{0, 0, 0},
                             {0, 0, 0},
                             {0, 1, 0},
                             {1, 1, 1}};

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
            {
                tetrominoCraftGrid[i][j] = tshape[i][j];
            }
    break;
    }

    default:
        qDebug() << "Invalid input to TetrominoMaker-Function";
}

//    for (int i = 0; i < 4; i++)
//        for (int j = 0; j < 3; j++)
//        {
//            std::cout << "position " << i << " and " << j << " is " << tetrominoCraftGrid[i][j] << "\n";
//        }

    return GameBlock;
}

int GamePlayMechanics::GetTetromino()
{
    //Select a random block out of the seven tetrominos. Discard if equal to last one.
    int currentTetromino = 0;
    int oldTetromino = 0;

    while (currentTetromino == oldTetromino) //This needs reworking, but isnt important right now. Need to keep track of oldTetromino in a way that is "stored"
    {
        currentTetromino = QRandomGenerator::global()->bounded(1, 8); //Include lowest, exclude largest;
        qDebug() << "current" << currentTetromino;
    }
    return currentTetromino;
}


void GamePlayMechanics::ActiveTetromino(int tetromino)
{
    //Drop the tetromino using a timer. Speed up gradually over time and or cleared lines
    //Allow user input, "W" to rotate, "A & D" to move sideways, "S" to drop faster and "Spacebar" to drop to bottom.

}


void GamePlayMechanics::ManageGameplayLines(bool gameField)
{
    //Check if one or more line is filled, and should be emptied

    //Could be made to handle "continuity" of blocks fallen, to make "gravity" work as in tetris games
    //Could be more easier and more "fun" if that handling isnt "correct", leading to more cleared lines?
}
