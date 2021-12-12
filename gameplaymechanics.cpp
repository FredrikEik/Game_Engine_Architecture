#include "gameplaymechanics.h"
#include "gameobjectmanager.h"
#include "gameobject.h"
#include "rendersystem.h"
#include <QRandomGenerator> //for random number generation

//Create the rules and gameplay of tetris

GamePlayMechanics::GamePlayMechanics(RenderSystem *renderSystemIn) : mRenderSystem {renderSystemIn}
{
    //Set up 2D array of squares of tetris
    bool gameField[6][9] = {};
    mGameObjectManager = &GameObjectManager::getInstance();
}

/*GameObject */ void GamePlayMechanics::TetrominoMaker(int tetromino)
{
    bool tetrominoCraftGrid[4][3] = {{0}};
    std::string GameBlockName = {};

//Find the design of the seven tetrominos given the input from 1 to 7.
switch (tetromino)
{
    case 1:
    {
        //Create line
        GameBlockName = "Line";

        bool line[4][3] = {{1, 0, 0}, //Tried being a bit fancy with the tetromino-creation,
                           {1, 0, 0}, //but i think its easier using 3d models and then using their vertices for collision
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
        GameBlockName = "Cube";

        bool block[4][3] = {{1, 1, 0},
                            {1, 1, 0},
                            {0, 0, 0},
                            {0, 0, 0}};

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
        GameBlockName = "ZigZag";

        bool sigzag[4][3] = {{1, 1, 0},
                             {0, 1, 1},
                             {0, 0, 0},
                             {0, 0, 0}};

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
        GameBlockName = "ZigZagL";

        bool revSigzag[4][3] = {{0, 1, 1},
                                {1, 1, 0},
                                {0, 0, 0},
                                {0, 0, 0}};

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
        GameBlockName = "L";

        bool Lshape[4][3] = {{1, 0, 0},
                             {1, 0, 0},
                             {1, 1, 0},
                             {0, 0, 0}};

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
        GameBlockName = "LR";

        bool revLshape[4][3] = {{0, 1, 0},
                                {0, 1, 0},
                                {1, 1, 0},
                                {0, 0, 0}};

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
        GameBlockName = "T";

        bool tshape[4][3] = {{0, 1, 0},
                             {1, 1, 1},
                             {0, 0, 0},
                             {0, 0, 0}};

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

    // I create a tetromino out og four gameobjects.
    GameObject *GameBlock;
    GameBlockName = "Tetromino"; //for testing purposes

    for (int i = 0; i < 4; i++) //Need to go through the entire tetrominoCraftGrid
    {
        for (int j = 0; j < 3; j++)
        {
            if (tetrominoCraftGrid[i][j] == true) //If there is something in the grid coordinate, create a block with the position.
            {                                     //Only four hits will be made.
                GameBlock = mGameObjectManager->addObject("Cube.obj");
                GameBlock->mTransform->mMatrix.translate(i, 0.0f, j);
                GameBlock->mTransform->mMatrix.scale(1.0f);
                GameBlock->mName = GameBlockName + std::to_string(i);
                qDebug() << "Tetrominoblock made";
            }
        }
    }
    mRenderSystem->mGameObjects.emplace_back(GameBlock);
//    qDebug() << "Testing variables, need a breakpoint";
//    return GameBlock;
}


int GamePlayMechanics::GetTetromino()
{
    //Select a random block out of the seven tetrominos. Discard if equal to last one.
    int currentTetromino = 0;
    int oldTetromino = 0; //Need to figure out a way to keep this between frame-calls. Now this runs every frame. Could use mGameObjects and search for names/id's.

    while (currentTetromino == oldTetromino) //This needs reworking, but isnt important right now. Need to keep track of oldTetromino in a way that is "stored" between function calls
    {
        currentTetromino = QRandomGenerator::global()->bounded(1, 8); //Include lowest, exclude largest;
        //qDebug() << "current" << currentTetromino;
    }
    return currentTetromino;
}


void GamePlayMechanics::ActiveTetromino(int tetromino)
{
    //Drop the tetromino towards bottom. Speed up gradually over time and or cleared lines
    //Allow user input, "W" to rotate, "A & D" to move sideways, "S" to drop faster and "Spacebar" to drop to bottom.

}


void GamePlayMechanics::ManageGameplayLines(bool gameField)
{
    //Check if one or more line is filled, and should be emptied

    //Could be made to handle "continuity" of blocks fallen, to make "gravity" work as in tetris games
    //Could be more easier and more "fun" if that handling isnt "correct", leading to more cleared lines?
}
