#ifndef GAMEPLAYMECHANICS_H
#define GAMEPLAYMECHANICS_H

//Forward declarations
class GameObject;
class GameObjectManager;
class RenderSystem;

/**
  Her foregår spill-logikken, i dette tilfellet til Tetris.
  Dette for å skille spillmotor og spill hvertfall delvis.
 */

class GamePlayMechanics
{
public:
    //Functions
    GamePlayMechanics(RenderSystem *renderSystemIn);
    int GetTetrominoNr();
    void MoveTetromino(int time);
    void ManageGameplayLines();
    /*GameObject*/ void TetrominoMaker(int tetromino);

    //Set up 2D array of squares of tetris
    const int horizontalIndex = 6;
    const int verticalIndex = 9;
    bool gameField[6][9] = {{0}};

    int gameSpeed = 10; //The higher the number, the faster the falling of tetrominoes happen.

    GameObjectManager *mGameObjectManager{nullptr};
    RenderSystem *mRenderSystem{nullptr};
};

#endif // GAMEPLAYMECHANICS_H
