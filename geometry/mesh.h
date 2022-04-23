#pragma once
#include<mygeo/vec.h>
#include"space.h"
#include"object.h"
#include"shapes.h"
#include<vector>
#include<algorithm>
struct Mesh : Object
{
    
};

struct TriangleMesh: public Mesh
{
    const int numTriangles,numVertices;
    std::vector<int> indices;
    std::vector<Point> p;
    std::vector<Normal> n;
    std::vector<MyGeo::Vec2f> uv;
    TriangleMesh(std::vector<int> indices,std::vector<Point> p,std::vector<Normal> n,std::vector<MyGeo::Vec2f> uv):
    indices{indices},p{p},n{n},uv{uv},numVertices{(int)indices.size()},numTriangles{(int)indices.size()/3} {}

    virtual bool hit(const Ray& ray, HitRecord& rec) const override;
    // {
    //     bool flag=false;
    //     for(int i=0;i!=numTriangles;++i)
    //     {
    //         Triangle triangle{*this,indices.data()+i*3};
    //         flag=triangle.hit(ray,rec);
    //     }
    //     return flag;
    // }
    
};


struct Triangle: public Shape
{
    const TriangleMesh& mesh;
    const int* indices;
    Normal normal;//todo

    Triangle(const TriangleMesh& mesh, const int* indices)
    : mesh{mesh},indices{indices},normal{mesh.p[indices[0]].v3.cross(mesh.p[indices[1]].v3)} 
    {}

    Triangle(const TriangleMesh& mesh, const int* indices,const Normal& normal)
    : mesh{mesh},indices{indices},normal{normal} 
    {}
    virtual BB3f bound() const override
    {
        const MyGeo::Vec3f& v0=mesh.p[indices[0]].v3;
        const MyGeo::Vec3f& v1=mesh.p[indices[1]].v3;
        const MyGeo::Vec3f& v2=mesh.p[indices[2]].v3;
        return BB3f{
            MyGeo::Vec3f{std::min({v0.x,v1.x,v2.x}),std::min({v0.y,v1.y,v2.y}),std::min({v0.z,v1.z,v2.z})},
            MyGeo::Vec3f{std::max({v0.x,v1.x,v2.x}),std::max({v0.y,v1.y,v2.y}),std::max({v0.z,v1.z,v2.z})}
        };
    }
    virtual float area() const override
    {
        return 1;
    }

    virtual bool hit(const Ray& ray, HitRecord& rec) const override
    {
        const MyGeo::Vec3f& v0=mesh.p[indices[0]].v3;
        const MyGeo::Vec3f& v1=mesh.p[indices[1]].v3;
        const MyGeo::Vec3f& v2=mesh.p[indices[2]].v3;

        MyGeo::Vec3f e1=v1-v0;
        MyGeo::Vec3f e2=v2-v0;
        MyGeo::Vec3f s=ray.source.v3-v0;
        MyGeo::Vec3f s1=ray.direction.v3.cross(e2);
        MyGeo::Vec3f s2=s.cross(e1);
        auto recidenom=1.0f/s1.dot(e1);
        auto t=s2.dot(e2)*recidenom;
        auto beta=s1.dot(s)*recidenom;
        auto gamma=s2.dot(ray.direction.v3)*recidenom;
        if(t>rec.tmin && t<rec.tmax && beta>0 && gamma>0 && beta+gamma<1.0f)
        {
            rec.t=t;
            rec.position=ray.at(t);
            rec.normal=normal;
            return true;
        }

        return false;
    }   
    
};

bool TriangleMesh::hit(const Ray& ray, HitRecord& rec) const 
{
    bool flag=false;
    for(int i=0;i!=numTriangles;++i)
    {
        Triangle triangle{*this,indices.data()+i*3};
        flag=triangle.hit(ray,rec);
    }
    return flag;
}


