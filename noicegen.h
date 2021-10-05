#ifndef NOICEGEN_H
#define NOICEGEN_H

#include <math.h>
// Copyright (C) 2003, 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)

class NoiceGen
{
public:
    NoiceGen();
    ~NoiceGen();
    enum NoiseQuality{
        QUALITY_FAST = 0,
        QUALITY_STD = 1,
        QUALITY_BEST =2
    };
    double GradientCoherentNoise3D (double x, double y, double z, int seed = 0,
        NoiseQuality noiseQuality = QUALITY_STD);
    double GradiantNoice3D(double fx, double fy, double fz, int ix, int iy, int iz, int seed = 0);
    int IntValueNoise3D (int x, int y, int z, int seed = 0);
    inline double MakeInt32Range (double n);
    double ValueCoherentNoise3D (double x, double y, double z, int seed = 0,
        NoiseQuality noiseQuality = QUALITY_STD);
    double ValueNoise3D (int x, int y, int z, int seed = 0);
};

#endif // NOICEGEN_H
