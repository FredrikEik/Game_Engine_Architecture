using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using ScriptInJin;
using ScriptInJin.Math;

namespace Game
{
    public class GameMode : Entity
    {
        uint unit;
        uint ball;
        uint obstacle;
        uint goal;
        bool bInitialized = false;
        public override void BeginPlay()
        {
            Console.WriteLine("GameMode's begin play called");

            unit = createDefaultEntity_Internal("../Saves/unit.json");
            ball = createDefaultEntity_Internal("../Saves/ball.json");
            obstacle = createDefaultEntity_Internal("../Saves/obstacle.json");
            goal = createDefaultEntity_Internal("../Saves/goal.json");

            addMaterialTexture_Internal(unit, "u_tex_diffuse1", "Assets/Dogling_D.png");
            addMaterialTexture_Internal(unit, "u_tex_specular1", "Assets/Dogling_S.png");

            Ball ballRef = (Ball)getObject_Internal(ball);
            ballRef.setPlayer(unit);
            Obstacle obstacleRef = (Obstacle)getObject_Internal(obstacle);
            obstacleRef.setBall(ball);

            Goal goalRef = (Goal)getObject_Internal(goal);
            goalRef.setBall(ball);

            Console.WriteLine("Obstacle id: " + obstacle);
            Transform.setPosition_internal(obstacle, new Vec3(100, 0, 140));
            //playSound_Internal();
        }
        public override void Update()
        {

        }
    }
}
