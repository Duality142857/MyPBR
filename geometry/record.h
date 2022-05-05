#pragma once
#include<mygeo/vec.h>
#include"space.h"
#include"transforms.h"

struct Primitive;

struct HitRecord
{
    float t;
    float tmin=0.f;
    float tmax=std::numeric_limits<float>::max();
    Point position;
    Normal normal;
    const Primitive* primitive=nullptr;
    
    void transform(const MatrixTransform& transform)
    {
        position=transform(position);
        normal=transform(normal);
    }
    void fixNormal(const MyGeo::Vec3f& raydirection)
    {
        if(normal.v3.dot(raydirection)>0) normal.v3=-normal.v3;
    }    

};