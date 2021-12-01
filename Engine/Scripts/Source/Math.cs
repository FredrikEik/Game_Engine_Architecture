﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

//This is used to setup internal calls
using System.Runtime.CompilerServices;

namespace ScriptInJin.Math
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec3
    {
        public float m_x;
        public float m_y;
        public float m_z;

        public Vec3(float x, float y, float z)
        {
            m_x = x;
            m_y = y;
            m_z = z;


        }

        public Vec3(float scalar)
        {
            m_x = m_y = m_z = scalar;
        }

        public static Vec3 operator +(Vec3 a, Vec3 b) => new Vec3(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
    }
}