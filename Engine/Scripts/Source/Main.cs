using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//This is used to setup internal calls
using System.Runtime.CompilerServices;

namespace ScriptInJin
{
    public abstract class Entity
    {

        public abstract void Update();
        public abstract void BeginPlay();

        //This indicates CSharp that the definition of this method is outside this project, as an Internal Call


    }

    public class Debug
    {

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void PrintMethod_Interal(string msg);
    }
}
