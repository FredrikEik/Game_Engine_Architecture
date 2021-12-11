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
    int GetTetromino();
    void ActiveTetromino(int tetromino);
    void ManageGameplayLines(bool gameField);
    std::vector<GameObject*> TetrominoMaker(int tetromino);

    GameObjectManager *mGameObjectManager{nullptr};
    RenderSystem *mRenderSystem{nullptr};
};

#endif // GAMEPLAYMECHANICS_H
