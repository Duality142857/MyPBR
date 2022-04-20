#include <iostream>
#include"geometry/bbox.h"
#include"geometry/ray.h"
#include"geometry/transforms.h"
#include"geometry/shapes.h"
#include"geometry/instance.h"
#include<mygeo/mat.h>

// struct vec3
// {
//     std::array<float,3> data;
// };

// struct mat3
// {
//     std::array<vec3,3> columns;
// };
// mat3 m{vec3{1,2,3},vec3{4,5,6},vec3{7,8,9}};


void test()
{
    MyGeo::Mat4f m{
        MyGeo::Vec4f{1,0,0,0},
        {1,0,1,0},
        {0,1,1,0},
        {1,0,0,2}
    };
    std::cout<<m;
    std::cout<<"det: "<<m.determinant()<<std::endl;
    std::cout<<MyGeo::invMat(m)*m<<std::endl;
    std::cout<<AR(m,0,0)<<std::endl;
}

int main(int argc, char** argv) 
{
    // test();return 0;
    Point p{1,0,0};
    auto t=translateMat({1,1,1});
    auto q=t(p);
    auto s=scaleMat({1,2,3});
    auto r=rotationMat({0,0,1},60);
    auto mix1=s*t;
    auto mix2=t*s;
    // std::cout<<mix1.m<<std::endl;
    // std::cout<<mix2.m<<std::endl;
    std::cout<<mix1.m<<std::endl;

    std::cout<<"det: "<<mix1.m.determinant()<<std::endl;
    std::cout<<mix2.m<<std::endl;

    std::cout<<"det: "<<mix2.m.determinant()<<std::endl;

    auto inverse=mix2.inverse();
    std::cout<<inverse.m<<std::endl;

    std::cout<<(mix2*inverse).m<<std::endl;


}


