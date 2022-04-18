#pragma once
#include<mygeo/vec.h>
struct HitRecord
{
    float t;
    float tmin=0.f;
    float tmax=std::numeric_limits<float>::max();

};