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

        public Vec3 getPosition()
        {
            return Transform.getTransform_internal(entityID); 
        }

        public void setPosition(Vec3 newPosition)
        {
            Transform.setPosition_internal(entityID, newPosition);
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

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static bool IsEntitySelected_internal(uint EntityID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static float getDeltaTime_Internal();
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static Entity getObject_Internal(uint EntityID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static uint createDefaultEntity_Internal(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static bool isOverlappingEntity_Internal(uint EntityA, uint EntityB);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void setVelocity_Internal(uint Entity, Vec3 newVelocity, bool isAdditive);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void setParticleActive_Internal(uint Entity, bool isActive);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void showHud_Internal(uint Entity, string path);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void removeHud_Internal(uint Entity);
    }

    public class Transform
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void Move_Internal(uint EntityID, Vec3 newLocation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static Vec3 getTransform_internal(uint EntityID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void setPosition_internal(uint EntityID, Vec3 newLocation);
    }

    public class Debug
    {

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void PrintMethod_Internal(string msg);
    }

    public class Input
    {
        public enum PRESSED_STATE
        {
            KEYSTATE_PRESSED,
            KEYSTATE_HELD,
            KEYSTATE_RELEASED,
        }

        public enum KEY_CODES
        {
            // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
            KEY_LMB = 0x00,
            KEY_RMB = 0x01,
            KEY_MMB = 0x02,
            KEY_MBACK = 0x03,
            KEY_MFORWARD = 0x04,
            KEY_A = 0x41,
            KEY_B = 0x42,
            KEY_C = 0x43,
            KEY_D = 0x44,
            KEY_E = 0x45,
            KEY_F = 0x46,
            KEY_G = 0x47,
            KEY_H = 0x48,
            KEY_I = 0x49,
            KEY_J = 0x4A,
            KEY_K = 0x4B,
            KEY_L = 0x4C,
            KEY_M = 0x4D,
            KEY_N = 0x4E,
            KEY_O = 0x4F,
            KEY_P = 0x50,
            KEY_Q = 0x51,
            KEY_R = 0x52,
            KEY_S = 0x53,
            KEY_T = 0x54,
            KEY_U = 0x55,
            KEY_V = 0x56,
            KEY_W = 0x57,
            KEY_X = 0x58,
            KEY_Y = 0x59,
            KEY_Z = 0x5A,
            KEY_ESCAPE = 0x1B
        };

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static Vec3 getCursorWorldPosition_Internal(uint entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static bool getMouseKeyState_Internal(KEY_CODES key, PRESSED_STATE state);
    }
}
