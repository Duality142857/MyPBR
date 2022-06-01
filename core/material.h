#pragma once
#include<mygeo/vec.h>
#include<optional>
struct Material
{
    virtual MyGeo::Vec3f getColor(float u,float v) const =0;     
    // virtual MyGeo::Vec3f emitColor(float u,float v) const =0;
    virtual MyGeo::Vec3f brdf(const MyGeo::Vec3f& lo, const MyGeo::Vec3f& li) const
    {
        return {0,0,0};
    }
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
    // virtual float reciPDF() const =0;
};

struct MonoLight : public Light
{
    MyGeo::Vec3f color;
    MonoLight(const MyGeo::Vec3f& color):color{color}{}
    virtual MyGeo::Vec3f getColor(float u,float v) const override
    {
        return color;
    }    
    // virtual float reciPDF() const override
    // {
    //     return 1
    // }

};


struct Lambertian : public Material
{
    MyGeo::Vec3f color;
    MyGeo::Vec3f albedo;
    MyGeo::Vec3f brdf(const MyGeo::Vec3f& lo, const MyGeo::Vec3f& li) const override
    {
        return color*ReciPi;
    }
    Lambertian(const MyGeo::Vec3f& color):color{color},albedo{0.8,0.8,0.8}{}
    Lambertian(const MyGeo::Vec3f& color, const MyGeo::Vec3f& albedo):color{color},albedo{albedo}{}
    // float pdf()

    // virtual bool ifemit() const override 
    // {
    //     return false;
    // }
    virtual MyGeo::Vec3f getColor(float u,float v) const override
    {
        return color;
    }
};
