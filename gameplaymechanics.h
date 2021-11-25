#ifndef GAMEPLAYMECHANICS_H
#define GAMEPLAYMECHANICS_H

//Forward declaration
class GameObject;
class GameObjectManager;
class RenderSystem;


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
