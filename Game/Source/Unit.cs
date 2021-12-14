using System;
using ScriptInJin;
using ScriptInJin.Math;

namespace Game
{
    public class Unit : Entity
    {
        Vec3 targetLocation;
        bool bShouldMove = false;
        float speed = 15;
        public override void BeginPlay()
        {
            Console.WriteLine(this.ToString());
            uint i = entityID;
            //Debug.PrintMethod_Internal("BPlay");
            Console.WriteLine(i);
            //Debug.PrintMethod_Internal("Fin");
            //Debug.PrintMethod_Interal("BPlay");
            //throw new NotImplementedException();
        }
        public Vec3 offset = new Vec3(0.0f, 0.001f, 0.0f);
        public override void Update()
        {
            //Console.WriteLine("Updating unit with id: " + entityID);
            //getObject_Internal(entityID);
            //Debug.PrintMethod_Internal("Update");
            //uint id = getObject_Internal(entityID).entityID;
            //Console.WriteLine("Object internal id: " + id);
            if (IsEntitySelected_internal(entityID))
            {

                //Console.WriteLine("Entity is selected");
                //Vec3 currentPos = Transform.getTransform_internal(entityID);
                //Console.WriteLine(currentPos.m_y);
                //Transform.Move_Internal(entityID, currentPos + offset);
                //Transform.Move_Internal(entityID, offset);

                if (Input.getMouseKeyState_Internal(Input.KEY_CODES.KEY_RMB, Input.PRESSED_STATE.KEYSTATE_PRESSED))
                {
                    targetLocation = Input.getCursorWorldPosition_Internal(entityID);
                    targetLocation.m_y = 0;
                    bShouldMove = true;
                    //Console.WriteLine(clickedPos.m_x + " y: " + clickedPos.m_y + " z: " + clickedPos.m_z);
                }
                //Debug.PrintMethod_Interal("update");
                //throw new NotImplementedException();
            }
            //else
            //    Console.WriteLine("Entity not selected");
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
