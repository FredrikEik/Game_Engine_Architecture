#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H
#include "camera.h"
#include "input.h"
#include "player.h"
#include "movementcomponent.h"
#include "enemy.h"
#include "particles.h"


class MovementSystem
{
public:
    MovementSystem();

    void update(Player *mPlayer,Camera *mCamera,Input mInput);
    void CheckActiveInput();
    void setCameraSpeed(Camera *mCurrentCamera,float value);
    void movePlayer(Player *mPlayer);
    void moveParticles(gsl::Vector3D mColor, std::vector<Particles*> mParticles, Player *mPlayer);
    void moveEnemy(int randNum, std::vector<Enemy*> mEnemies);

private:
    CollisionSystem* mColSystem{nullptr};
    ShapeFactory* mFactory{nullptr};

};

#endif // MOVEMENTSYSTEM_H
