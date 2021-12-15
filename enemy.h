#ifndef ENEMY_H
#define ENEMY_H

/**
  Enemy-klassen har en konstruktør som tar inn en shapeFactory-peker.
  Dette er for at vi skal kunne hente ut riktig mesh, og sette på riktig
  NameComponent-verdier.
  Vi legger også til en MovementComponent her, siden den ikke arver dette
  fra VisualObject.
 */

class Enemy : public VisualObject
{
public:
    Enemy(ShapeFactory* f);
    ~Enemy(){};
    MovementComponent* mMoveComp;
};

#endif // ENEMY_H
