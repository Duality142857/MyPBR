#pragma once
#include<mygeo/mat.h>
#include"space.h"
#include"utils.h"

struct Transform 
{
    virtual Point operator()(const Point& p) const =0;
    // virtual Vect operator()(const Vect& v) const
    // Transform transpose()
    // {
    //     return {m.transpose()};
    // }
    // Transform operator*(const Transform& B)
    // {
    //     return {m*B.m};
    // }
};

struct MatrixTransform: public Transform
{
    MyGeo::Mat4f m;
    // MatrixTransform(const MyGeo::Mat4f& m):m{m}{}
    MatrixTransform(const MyGeo::Mat4f& m):m{m}{}


    //act on Point
    virtual Point operator()(const Point& p) const
    {
        return {m*p.v4};
    }

    //act on Vector
    virtual Vect operator()(const Vect& v) const
    {
        return {m*v.v4};
    }
};

static MatrixTransform scaleMat(const MyGeo::Vec3f& k)
{
    return {MyGeo::Mat4f{MyGeo::Vec4f{k[0],0,0,0},{0,k[1],0,0},{0,0,k[2],0},{0,0,0,1}}};
}

static MatrixTransform rotationMat(const MyGeo::Vec3f& axis, float angle)
{
    MyGeo::Vec3f nAxis=axis;
    nAxis.normalize();
    // std::cout<<nAxis<<std::endl;
    float C=std::cos(toRad(angle));
    float R=1-C;
    float S=std::sin(toRad(angle));
    float a1=nAxis.x;
    float a2=nAxis.y;
    float a3=nAxis.z;
    return {MyGeo::Mat4f{MyGeo::Vec4f{C+R*a1*a1,R*a1*a2+S*a3,R*a1*a3-S*a2,0},
            MyGeo::Vec4f{R*a1*a2-S*a3,C+R*a2*a2,R*a2*a3+S*a1,0},
            MyGeo::Vec4f{R*a1*a3+S*a2,R*a2*a3-S*a1,C+R*a3*a3,0},
            MyGeo::Vec4f{0,0,0,1}
            }};   
}

struct Translate : public Transform
{
    MyGeo::Vec4f tVec;
    Translate(const MyGeo::Vec3f& transVec):tVec{transVec,1.f}{}
    Translate(const MyGeo::Vec4f& transVec):tVec{transVec}{}
    Translate(float x,float y,float z):tVec{x,y,z,1.f}{}


    //act on Point, pure translation does nothing on vector
    virtual Point operator()(const Point& p) const override
    {
        return Point{p.v3+tVec.head};
    }
    
    friend Translate operator*(const Translate& t1 ,const Translate& t2)    
    {
        return Translate{t1.tVec+t2.tVec};
    }

    //rotate, then translate
    friend MatrixTransform operator*(const Translate& tl, const MatrixTransform& tm)
    {
        auto res=tm;
        res.m.col[3]+=tl.tVec;
        return res;
    }

    friend MatrixTransform operator*(const MatrixTransform& tm, const Translate& tl)
    {   
        MyGeo::Mat4f transMat{MyGeo::Vec4f{1,0,0,0},MyGeo::Vec4f{1,0,0,0},MyGeo::Vec4f{1,0,0,0},tl.tVec};
        return {tm.m*transMat};
    }
};
