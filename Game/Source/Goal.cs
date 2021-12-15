using System;
using ScriptInJin;
using ScriptInJin.Math;

namespace Game
{
    public class Goal : Entity
    {
        uint ball;
        bool hudShowing = false;
        float hudTime = 0;
        public override void BeginPlay()
        {

        }
        public override void Update()
        {
            if (hudShowing)
            {
                hudTime -= getDeltaTime_Internal();
                if (hudTime <= 0)
                {
                    hudShowing = false;
                    removeHud_Internal(entityID);
                }
            }
            //Console.WriteLine("Updating ball unit with id: " + entityID);
            if (isOverlappingEntity_Internal(entityID, ball))
            {
                Console.WriteLine("Ball overlapping goal in c#");
                uint starExplosion = createDefaultEntity_Internal("../saves/win.json");
                Transform.setPosition_internal(starExplosion, getPosition() + new Vec3(-1.5f, 1, 3));
                setParticleActive_Internal(starExplosion, true);
                Ball ballRef = (Ball)getObject_Internal(ball);
                ballRef.Reset();


                showHud_Internal(entityID, "Assets/winHud.png");
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
