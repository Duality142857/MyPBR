#pragma once
#include<mygeo/vec.h>
#include"space.h"

struct Ray
{
    // const MyGeo::Vec3f& source;
    // const MyGeo::Vec3f& direction;
    const Point source;
    const Vect direction;
    Ray(const MyGeo::Vec3f& source, const MyGeo::Vec3f& direction):source{source},direction{direction}{}
    Ray(const Point& source, const Vect& direction):source{source},direction{direction}{}


    Point at(float t) const 
    {
        return {source.v3+t*direction.v3};
    }
    
};
