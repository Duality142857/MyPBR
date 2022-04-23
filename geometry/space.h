#pragma once
#include<mygeo/vec.h>
#include<iostream>

struct Vect
{
    Vect(const MyGeo::Vec3f& v):v4{v,0.f}{}
    Vect(const MyGeo::Vec4f& v):v4{v}{}
    Vect(const Vect& v):v4{v.v4}{}
    Vect(float x,float y,float z):v4{x,y,z,0.f}{}
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
    Vect cross(const Vect& v) const 
    {
        return {v3.cross(v.v3)};
    }
    Vect& normalize() 
    {
        v3.normalize();
        return *this;
    }
};

struct Point
{
    Point(const MyGeo::Vec3f& v):v4{v,1.f}{}
    Point(const MyGeo::Vec4f& v):v4{v}{}
    Point(const Point& p):v4{p.v4}{}
    Point(float x,float y,float z):v4{x,y,z,1.f}{}
    Point operator=(const Point& p)
    {
        v4=p.v4;
    }
    Point operator+(const Vect& displacement) const
    {
        return {v4+displacement.v4};
    }

    Vect operator-(const Point& p) const 
    {
        return {v4-p.v4};
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






struct Normal
{
    Normal(const MyGeo::Vec3f& v):v4{v,0.f}{}
    Normal(const MyGeo::Vec4f& v):v4{v}{}
    Normal(const Normal& v):v4{v.v4}{}
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