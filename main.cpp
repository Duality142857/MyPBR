#include <iostream>
#include"geometry/bbox.h"
#include"geometry/ray.h"
#include"geometry/transforms.h"
#include"geometry/shapes.h"
int main(int argc, char** argv) 
{
    Point p{1,0,0};
    Translate t{1,2,3};
    auto q=t(p);
    auto s=scaleMat({1,2,3});
    auto r=rotationMat({0,1,0},45);
    auto mix1=s*t;
    auto mix2=t*s;
    // std::cout<<mix1.m<<std::endl;
    // std::cout<<mix2.m<<std::endl;
    std::cout<<r.m<<std::endl;

}


