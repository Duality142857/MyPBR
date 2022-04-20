#pragma once
#include<mygeo/vec.h>
#include<iostream>
struct Point
{
    Point(const MyGeo::Vec3f& v):v4{v,1.f}{}
    Point(const MyGeo::Vec4f& v):v4{v}{}
    Point(const Point& p):v4{p.v4}{}
    Point(float x,float y,float z):v4{x,y,z,0.f}{}
    Point operator=(const Point& p)
    {
        v4=p.v4;
    }
    union 
    {
        MyGeo::Vec3f v3;
        MyGeo::Vec4f v4;
        struct 
        {
           float x,y,z,w;
        };
    };
    friend std::ostream& operator<<(std::ostream& out,const Point& p)
    {
        out<<p.v3<<std::endl;
    }
};

struct Vect
{
    Vect(const MyGeo::Vec3f& v):v4{v,0.f}{}
    Vect(const MyGeo::Vec4f& v):v4{v}{}
    Vect(const Vect& v):v4{v.v4}{}
    Vect operator=(const Vect& v)
    {
        v4=v.v4;
    }
    union 
    {
        MyGeo::Vec3f v3;
        MyGeo::Vec4f v4;
        struct 
        {
           float x,y,z,w;
        };
        
    };
};

struct Normal
{
    Normal(const MyGeo::Vec3f& v):v4{v,0.f}{}
    Normal(const MyGeo::Vec4f& v):v4{v}{}
    Normal(const Vect& v):v4{v.v4}{}
    Normal operator=(const Normal& v)
    {
        v4=v.v4;
    }
    union 
    {
        MyGeo::Vec3f v3;
        MyGeo::Vec4f v4;
        struct 
        {
           float x,y,z,w;
        };
    };
};