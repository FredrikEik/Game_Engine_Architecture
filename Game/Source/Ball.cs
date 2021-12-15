using System;
using ScriptInJin;
using ScriptInJin.Math;

namespace Game
{
    public class Ball : Entity
    {
        uint player;
        Vec3 startPosition;
        public override void BeginPlay()
        {
            startPosition = getPosition();
        }
        public override void Update()
        {
            //Console.WriteLine("Updating ball unit with id: " + entityID);
            if(isOverlappingEntity_Internal(entityID, player))
            {
                Unit overlappedUnit = (Unit)getObject_Internal(player);
                Vec3 playerPosition = overlappedUnit.getPosition();
                playerPosition.m_y = 0.5f;
                Vec3 collisionDirection =  getPosition() - playerPosition;
                //Vec3 collisionDirection = playerPosition - getPosition();
                collisionDirection.Normalize();
                playSound_Internal();

                setVelocity_Internal(entityID, collisionDirection * 10, false);
            }
        }
        public void setPlayer(uint unit)
        {
            player = unit;
            Console.WriteLine("Player set in ball");
        }

        public void Reset()
        {
            setPosition(startPosition);
            setVelocity_Internal(entityID, new Vec3(0, 0, 0), false);
        }
    }
}
 