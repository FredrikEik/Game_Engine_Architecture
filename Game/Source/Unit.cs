using System;
using ScriptInJin;
using ScriptInJin.Math;

namespace Game
{
    public class Unit : Entity
    {
        Vec3 targetLocation;
        bool bShouldMove = false;
        float speed = 5;
        public override void BeginPlay()
        {
            Console.WriteLine(this.ToString());
            uint i = entityID;
            Console.WriteLine(i);
        }
        public Vec3 offset = new Vec3(0.0f, 0.001f, 0.0f);
        public override void Update()
        {
            if (IsEntitySelected_internal(entityID))
            {
                if (Input.getMouseKeyState_Internal(Input.KEY_CODES.KEY_RMB, Input.PRESSED_STATE.KEYSTATE_PRESSED))
                {
                    targetLocation = Input.getCursorWorldPosition_Internal(entityID);
                    targetLocation.m_y = 0;
                    bShouldMove = true;
                }

            }

            Move();
        }

        void Move()
        {
            if(bShouldMove)
            {
                Vec3 currentPos = Transform.getTransform_internal(entityID);

                Vec3 direction = targetLocation - currentPos;

                if (direction.Length() <= 0.1)
                    bShouldMove = false;

                direction.Normalize();
                float deltaTime = getDeltaTime_Internal();

                Transform.Move_Internal(entityID, direction * speed * deltaTime);
            }
        }
    }
}
