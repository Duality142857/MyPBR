#pragma once

struct Primitive : Object
{
    std::shared_ptr<Shape> shape;
    Primitive(std::shared_ptr<Shape> shape):shape{shape}{}
    // std::shared_ptr<Material> material;
    // std::shared_ptr
    virtual BB3f bound() const override
    {
        return shape->bound();
    }
    virtual bool hit(const Ray& ray,HitRecord& rec) const override
    {
        if(!shape->hit(ray,rec)) return false;
        rec.primitive=this;
    }
};


