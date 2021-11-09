#ifndef GAMEPLAYMECHANICS_H
#define GAMEPLAYMECHANICS_H

//Forward declaration
class GameObject;
class GameObjectManager;


class GamePlayMechanics
{
public:
    //Functions
    GamePlayMechanics();
    int GetTetromino();
    void ActiveTetromino(int tetromino);
    void ManageGameplayLines(bool gameField);
    GameObject* TetrominoMaker(int tetromino);

    GameObjectManager *mGameObjectManager{nullptr};
};

#endif // GAMEPLAYMECHANICS_H
