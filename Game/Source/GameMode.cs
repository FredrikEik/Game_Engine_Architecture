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
            ////await Task.Delay(10);
            unit = createDefaultEntity_Internal("../Saves/unit.json");
            ball = createDefaultEntity_Internal("../Saves/ball.json");
            //obstacles.Add(createDefaultEntity_Internal("../Saves/obstacle.json"));
            obstacle = createDefaultEntity_Internal("../Saves/obstacle.json");
            goal = createDefaultEntity_Internal("../Saves/goal.json");

            Ball ballRef = (Ball)getObject_Internal(ball);
            ballRef.setPlayer(unit);
            Obstacle obstacleRef = (Obstacle)getObject_Internal(obstacle);
            obstacleRef.setBall(ball);

            Goal goalRef = (Goal)getObject_Internal(goal);
            goalRef.setBall(ball);
            //obstacleRef.setPosition(new Vec3(20, 0, 20));
            //Vec3 obstaclePosition = Vec3(25.0, 0.0, 25.0);
            Console.WriteLine("Obstacle id: " + obstacle);
            //Console.WriteLine(getPosition().Length());
            Transform.setPosition_internal(obstacle, new Vec3(80, 0, 140));
            //Transform.Move_Internal(7, new Vec3(25, 0, 25));

        }
        public override void Update()
        {
            //Console.WriteLine("GameMode's update called");

            if (bInitialized == false)
            {
                Console.WriteLine("Obstacle id: " + obstacle);
                //Transform.setPosition_internal(obstacle, new Vec3(25, 0, 25));
                bInitialized = true;
                //Ball ballRef = (Ball)getObject_Internal(ball);
                //ballRef.setPlayer(unit);
                //Obstacle obstacleRef = (Obstacle)getObject_Internal(obstacles[0]);
                //obstacleRef.setBall(ball);
                ////Vec3 obstaclePosition = Vec3(25.0, 0.0, 25.0);

                //Transform.setPosition_internal(obstacles[0], new Vec3(25, 0, 25));
                //bInitialized = true;
            }
        }
    }
}
