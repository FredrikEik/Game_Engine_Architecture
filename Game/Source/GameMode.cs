using System;
using ScriptInJin;
using ScriptInJin.Math;

namespace Game
{
    public class GameMode : Entity
    {
        uint unit;
        uint ball;
        public override void BeginPlay()
        {
            Console.WriteLine("GameMode's begin play called");
            unit = createDefaultEntity_Internal("../Saves/unit.json");
            ball = createDefaultEntity_Internal("../Saves/ball.json");
            Ball ballRef = (Ball)getObject_Internal(ball);
            ballRef.setPlayer(unit);
        }
        public override void Update()
        {
            //Console.WriteLine("GameMode's update called");

        }
    }
}
