using System;
using ScriptInJin;
using ScriptInJin.Math;

namespace Game
{
    public class Goal : Entity
    {
        uint ball;
        public override void BeginPlay()
        {

        }
        public override void Update()
        {
            //Console.WriteLine("Updating ball unit with id: " + entityID);
            if(isOverlappingEntity_Internal(entityID, ball))
            {
                Console.WriteLine("Ball overlapping goal in c#");
                uint starExplosion = createDefaultEntity_Internal("../saves/win.json");
                Transform.setPosition_internal(starExplosion, getPosition() + new Vec3(-1.5f, 1, 3));
                setParticleActive_Internal(starExplosion, true);
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
