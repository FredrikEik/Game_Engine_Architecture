using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using ScriptInJin.Math;

//This is used to setup internal calls
using System.Runtime.CompilerServices;

namespace ScriptInJin
{
    public abstract class Entity
    {
        public uint entityID {
            
            get { return getEntityID_internal(this, ToString());}
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static uint getEntityID_internal(Entity ptr, string className);

        // used to set and get entityID
        private UIntPtr native_handle = (UIntPtr)0;

        public abstract void Update();
        public abstract void BeginPlay();

        //This indicates CSharp that the definition of this method is outside this project, as an Internal Call
        //[MethodImpl(MethodImplOptions.InternalCall)]
        //extern public static uint getEntityID_Internal(UIntPtr native_handle);

    }

    public class Transform
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void Move_Internal(uint entity, Vec3 newLocation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static Vec3 getTransform_internal(uint entity);
    }

    public class Debug
    {

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void PrintMethod_Internal(string msg);
    }
}
