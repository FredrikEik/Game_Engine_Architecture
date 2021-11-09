#ifndef GAMEPLAYMECHANICS_H
#define GAMEPLAYMECHANICS_H

//Forward declaration
class GameObject;


class GamePlayMechanics
{
public:
    GamePlayMechanics();
    int GetTetromino();
    void ActiveTetromino(int tetromino);
    void ManageGameplayLines(bool gameField);
    GameObject* TetrominoMaker(int tetromino);
};

#endif // GAMEPLAYMECHANICS_H
