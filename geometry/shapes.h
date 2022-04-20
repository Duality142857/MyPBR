#pragma once
#include"bbox.h"
#include"ray.h"
#include"record.h"
#include"transforms.h"
#include"utils.h"
#include"object.h"






struct Shape : public Object
{
    // const Transform *model, *model_inv;
    virtual BB3f bound() const =0;
    // virtual BB3f worldBound() const;
    virtual float area() const =0;
    // virtual bool hit(const Ray& ray, HitRecord& rec) const =0;
};

struct Sphere: Shape
{
    const float radius,radius2;

    Sphere(float radius):radius{radius},radius2{radius*radius}{}

    float area() const override {return 4.f*Pi*radius*radius;}
    
    bool hit(const Ray& ray, HitRecord& rec) const override
    {
        if(!bound().hit(ray)) return false;
        MyGeo::Vec3f l=ray.source.v3;
        float a=ray.direction.v3.norm2();
        float b=2*ray.direction.v3.dot(l);
        float c=l.norm2()-radius2;

        auto solution=solveQuadratic(a,b,c);
        if(!solution.has_value()) return false;
        auto [x1,x2]=solution.value();

        if(x2<=rec.tmin) return false;
        auto t=x1<rec.tmin?x2:x1;
        if(t>rec.tmax) return false;

        rec.t=t;
        rec.position=ray.at(t);
        return true;
    }
    virtual BB3f bound() const override
    {
        return BB3f{{-radius,-radius,-radius},{radius,radius,radius}};
    }
    // virtual BB3f worldBound() const
};
