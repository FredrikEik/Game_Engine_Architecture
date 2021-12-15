using System;
using ScriptInJin;
using ScriptInJin.Math;

namespace Game
{
    public class Obstacle : Entity
    {
        uint ball;
        public override void BeginPlay()
        {
            Console.WriteLine("Obstacle begin play");
            //setPosition(new Vec3(25, 0, 25));
        }
        public override void Update()
        {
            //Console.WriteLine("Updating ball unit with id: " + entityID);
            if(isOverlappingEntity_Internal(entityID, ball))
            {
                Console.WriteLine("Ball overlapping obstacle in c#");
                uint explosion = createDefaultEntity_Internal("../saves/explosion.json");
                Transform.setPosition_internal(explosion, getPosition()+new Vec3(-1.5f, 1, 3));
                //Transform.setPosition_internal(explosion, Transform.getTransform_internal(ball));
                setParticleActive_Internal(explosion, true);
                Ball ballRef = (Ball)getObject_Internal(ball);
                ballRef.Reset();
            }
        }

        public void setBall(uint id)
        {
            ball = id;
        }
    }
}
