#pragma once
#include<mygeo/vec.h>

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





    friend BBox Union(const BBox& a, const BBox& b)
    {
        return BBox(MyGeo::mixMin(a,b),MyGeo::mixMax(a,b));
    }

    friend BBox Intersect(const BBox& a, const BBox& b)
    {
        return BBox(MyGeo::mixMax(a,b),MyGeo::mixMin(a,b));
    }
};

