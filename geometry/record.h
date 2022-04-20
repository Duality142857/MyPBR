#pragma once
#include<mygeo/vec.h>
#include"space.h"
#include"transforms.h"
struct HitRecord
{
    float t;
    float tmin=0.f;
    float tmax=std::numeric_limits<float>::max();
    Point position;
    Vect normal;
    void transform(const MatrixTransform& transform)
    {
        position=transform(position);
        normal=transform(normal);
    }

};