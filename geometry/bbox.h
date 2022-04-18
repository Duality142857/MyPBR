#pragma once
#include<mygeo/vec.h>
#include"ray.h"

template<class T, int N>
struct BBox
{
    MyGeo::Vec<T,N> min,max;
    BBox(const MyGeo::Vec<T,N>& min,const MyGeo::Vec<T,N>& max):min{min},max{max}{}
    
    MyGeo::Vec<T,N> diagonal()
    {
        return max-min;
    }
    
    int mainAxis()
    {
        MyGeo::Vec<T,N> diag=diagonal();
        int k=0;
        if(N==1) return 0;
        if(diag.y>diag.x)
        {
            k=1;
            if(N==2) return k;
            if(diag.z>diag.y) k=2;
        }
        else if(diag.z>diag.x) k=2;
        return k;
    }

    bool hit(const Ray& ray)
    {
        float tmin=0,tmax=std::numeric_limits<float>::max();
        for(int i=0;i!=N;++i)
        {
            auto recid=1.0f/ray.direction[i];
            auto t0=(min[i]-ray.source[i])*recid;
            auto t1=(max[i]-ray.source[i])*recid;

            if(recid<0) std::swap(t0,t1);

            if(t0>tmin) tmin=t0;
            if(t1<tmax) tmax=t1;
            if(t0>=t1) return false;
        }
        return true;
    }



    friend BBox Union(const BBox& a, const BBox& b)
    {
        return BBox(MyGeo::mixMin(a,b),MyGeo::mixMax(a,b));
    }

    friend BBox Intersect(const BBox& a, const BBox& b)
    {
        return BBox(MyGeo::mixMax(a,b),MyGeo::mixMin(a,b));
    }
};

using BB3f=BBox<float,3>;
