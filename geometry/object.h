#pragma once
#include"ray.h"
#include"record.h"
struct Object
{
    virtual bool hit(const Ray& ray, HitRecord& rec) const =0;

};