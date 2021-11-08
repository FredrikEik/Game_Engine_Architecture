#ifndef GAMEPLAYMECHANICS_H
#define GAMEPLAYMECHANICS_H


class GamePlayMechanics
{
public:
    GamePlayMechanics();
    int GetTetromino();
    void ActiveTetromino(int tetromino);
    void ManageGameplayLines(bool gameField);
};

#endif // GAMEPLAYMECHANICS_H
