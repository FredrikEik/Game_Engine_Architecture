using System;
using ScriptInJin;
using ScriptInJin.Math;

namespace Game
{
    public class Ball : Entity
    {
        uint player;
        public override void BeginPlay()
        {

        }
        public override void Update()
        {
            //Console.WriteLine("Updating ball unit with id: " + entityID);
            if(isOverlappingEntity_Internal(entityID, player))
            {
                Unit overlappedUnit = (Unit)getObject_Internal(player);
                Vec3 playerPosition = overlappedUnit.getPosition();

                Vec3 collisionDirection = playerPosition - getPosition();
                collisionDirection.Normalize();
                

                setVelocity_Internal(entityID, collisionDirection * -10, false);
            }
        }
        public void setPlayer(uint unit)
        {
            player = unit;
            Console.WriteLine("Player set in ball");
        }
    }
}
 