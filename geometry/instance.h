#pragma once
#include"shapes.h"
#include"transforms.h"
#include"primitives.h"
#include<memory>
struct Instance: public Object
{
    std::shared_ptr<Primitive> primitive;
    MatrixTransform transform;
    MatrixTransform transform_inv;
    virtual BB3f bound() const override
    {
        return transform(primitive->bound());
    }

    Instance(const MatrixTransform& transform, std::shared_ptr<Primitive> primitive):transform{transform},primitive{primitive},transform_inv{transform.inverse()}
    {}

    virtual bool hit(const Ray& ray, HitRecord& rec) const override
    {
        auto flag=hit(transform(ray),rec);
        rec.transform(transform_inv);
        return flag;
    }
    
};
