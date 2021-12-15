using System;
using ScriptInJin;
using ScriptInJin.Math;

namespace Game
{
    public class Obstacle : Entity
    {
        uint ball;
        bool hudShowing = false;
        float hudTime = 0;
        public override void BeginPlay()
        {
            Console.WriteLine("Obstacle begin play");
            //setPosition(new Vec3(25, 0, 25));
        }
        public override void Update()
        {
            if(hudShowing)
            {
                hudTime -= getDeltaTime_Internal();
                if(hudTime<=0)
                {
                    hudShowing = false;
                    removeHud_Internal(entityID);
                }
            }

            if(isOverlappingEntity_Internal(entityID, ball))
            {
                Console.WriteLine("Ball overlapping obstacle in c#");
                uint explosion = createDefaultEntity_Internal("../saves/explosion.json");
                Transform.setPosition_internal(explosion, getPosition()+new Vec3(-1.5f, 1, 3));
                //Transform.setPosition_internal(explosion, Transform.getTransform_internal(ball));
                setParticleActive_Internal(explosion, true);
                Ball ballRef = (Ball)getObject_Internal(ball);
                ballRef.Reset();

                showHud_Internal(entityID, "Assets/loseHud.png");
                hudShowing = true;
                hudTime = 3.0f;
            }
        }

        public void setBall(uint id)
        {
            ball = id;
        }
    }
}
