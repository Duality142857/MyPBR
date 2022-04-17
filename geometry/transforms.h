#pragma once
#include<mygeo/mat.h>
struct Transform 
{

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
    MatrixTransform(const MyGeo::Mat4f& m):m{m}{}
};


struct Translate : public Transform
{
    const MyGeo::Vec4f& tVec;
    Translate(const MyGeo::Vec4f& tVec):tVec{tVec}{}
    friend Translate operator*(const Translate& t1 ,const Translate& t2)    
    {
        return Translate{t1.tVec+t2.tVec};
    }

    //rotate, then translate
    friend Transform operator*(const Translate& tl, const MatrixTransform& tm)
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
