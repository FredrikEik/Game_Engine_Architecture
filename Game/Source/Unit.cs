using System;
using ScriptInJin;
using ScriptInJin.Math;

namespace Game
{
    public class Unit : Entity
    {
        
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
        public Vec3 offset = new Vec3(0.0f, 0.01f, 0.0f);
        public override void Update()
        {

            //check if entity is selected 
            Vec3 currentPos = Transform.getTransform_internal(entityID);
            Console.WriteLine(currentPos.m_y);
            Transform.Move_Internal(entityID, currentPos + offset);

            //Debug.PrintMethod_Interal("update");
            //throw new NotImplementedException();
        }
    }
}
