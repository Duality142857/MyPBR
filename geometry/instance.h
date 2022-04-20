#pragma once
#include"shapes.h"
#include"transforms.h"
#include<memory>
struct Instance: public Object
{
    std::shared_ptr<Object> obj;
    MatrixTransform transform;
    MatrixTransform transform_inv;

    Instance(const MatrixTransform& transform, std::shared_ptr<Object> obj):transform{transform},obj{obj},transform_inv{transform.inverse()}
    {}

    virtual bool hit(const Ray& ray, HitRecord& rec) const override
    {
        auto flag=hit(transform(ray),rec);
        rec.transform(transform_inv);
        return flag;
    }
    
    
};
