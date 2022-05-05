#include <iostream>
#include"geometry/bbox.h"
#include"geometry/ray.h"
#include"geometry/transforms.h"
#include"geometry/shapes.h"
#include"geometry/instance.h"
#include"geometry/mesh.h"
#include"geometry/camera.h"
#include"geometry/aggregates.h"
#include"color/spectrum.h"
#include<mygeo/mat.h>
#include<algorithm>

// struct vec3
// {
//     std::array<float,3> data;
// };

// struct mat3
// {
//     std::array<vec3,3> columns;
// };
// mat3 m{vec3{1,2,3},vec3{4,5,6},vec3{7,8,9}};

        // float fov,
        // const MatrixTransform& cam2world, 
        // const MatrixTransform& cam2scr,
        // const BB2f& screen,
        // Film& film,
        // float lensRadius,
        // float focalDistance
void test()
{
    MyGeo::Mat4f m{
        MyGeo::Vec4f{1,0,0,0},
        {1,0,1,0},
        {0,1,1,0},
        {1,0,0,2}
    };
    // std::cout<<m;
    // std::cout<<"det: "<<m.determinant()<<std::endl;
    // std::cout<<MyGeo::invMat(m)*m<<std::endl;
    // std::cout<<AR(m,0,0)<<std::endl;

    // std::cout<<std::max({1.f,2.f,3.f})<<std::endl;
        // Point position,
        // Point lookat,
        // Vect up,
        // float fov,
        // float aspectRatio,
        // // const MatrixTransform& cam2world, 
        // // const MatrixTransform& cam2scr,
        // const BB2f& screen,
        // const Film& film,
        // float lensRadius,
        // float focalDistance
    PerspectiveCamera cam{Point{0,0,-1},Point{0,0,0},Vect{0,1,0},45,1.f,BB2f{{-1.f,-1.f},{1.f,1.f}},Film{MyGeo::Vec2i{800,800}},0.f,1.f};
    std::cout<<cam.genRay(800,800);
//     std::cout<<"next ray"<<std::endl;
    // std::cout<<cam.genRay(400,400);
}

void boxTest()
{
    BB3f box{{0,0,0},{1,1,1}};
    std::cout<<box;
    MatrixTransform mt{rotationMat({0,0,1},45)};
    std::cout<<mt(box);
}




void aggregateTest()
{
    auto primitive=std::make_shared<Primitive>(std::make_shared<Sphere>(5));
    Instance instance{rotationMat({0,0,1},0),primitive};
    std::cout<<instance.bound();
    
}

int main(int argc, char** argv) 
{
    aggregateTest();return 0;
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


