#pragma once
#include<mygeo/vec.h>
#include<optional>
struct Material
{
    virtual MyGeo::Vec3f getColor(float u,float v) const =0;     
    // virtual MyGeo::Vec3f emitColor(float u,float v) const =0;
 
    virtual bool ifemit() const 
    {
        return false;
    }
};

struct Light : public Material
{
    virtual bool ifemit() const override 
    {
        return true;
    }
};

struct MonoLight : public Light
{
    MyGeo::Vec3f color;
    MonoLight(const MyGeo::Vec3f& color):color{color}{}
    virtual MyGeo::Vec3f getColor(float u,float v) const override
    {
        return color;
    }    

};


struct Lambertian : public Material
{
    MyGeo::Vec3f color;
    Lambertian(const MyGeo::Vec3f& color):color{color}{}
    // virtual bool ifemit() const override 
    // {
    //     return false;
    // }
    virtual MyGeo::Vec3f getColor(float u,float v) const override
    {
        return color;
    }
};
