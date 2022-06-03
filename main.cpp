#include <iostream>
#include"geometry/bbox.h"
#include"geometry/ray.h"
#include"geometry/transforms.h"
#include"geometry/shapes.h"
#include"geometry/instance.h"
#include"geometry/mesh.h"
#include"core/camera.h"
#include"geometry/aggregates.h"
#include"color/spectrum.h"
#include"core/sampler.h"
#include"core/filter.h"
#include"core/material.h"
#include"core/object.h"
#include"core/canvas.h"
#include<mygeo/mat.h>
#include<algorithm>
#include"implement/mesh.inpl"


static std::shared_ptr<Material> genLambertian(const MyGeo::Vec3f& color)
{
    return std::make_shared<Lambertian>(color);
}

static std::shared_ptr<Material> genMonoLight(const MyGeo::Vec3f& color)
{
    return std::make_shared<MonoLight>(color);
}

static std::shared_ptr<Shape> genSphere(float radius)
{
    return std::make_shared<Sphere>(radius);
}

static std::shared_ptr<Shape> genCube(const MyGeo::Vec3f& scale)
{
    return std::make_shared<Cube>(scale);
}

template<class A,class B, class... Args>
std::shared_ptr<A> genShared(const Args&... args)
{
    return std::make_shared<B>(args...);
}

void renderTest()
{
    auto lambertian_white=genShared<Material,Lambertian>(MyGeo::Vec3f{1,1,1});
    auto lambertian_green=genShared<Material,Lambertian>(MyGeo::Vec3f{0,1,0});
    auto lambertian_red=genShared<Material,Lambertian>(MyGeo::Vec3f{1,0,0});
    auto lambertian_blue=genShared<Material,Lambertian>(MyGeo::Vec3f{0,0,1});
    auto lambertian_yellow=genShared<Material,Lambertian>(MyGeo::Vec3f{1,1,0});

    auto mirror=genShared<Material,Mirror>(MyGeo::Vec3f{1,1,1});
    auto dielectric=genShared<Material,Dielectric>(1.6f);




auto lightColor=8.0f * MyGeo::Vec3f{0.747f+0.058f, 0.747f+0.258f, 0.747f} + 15.6f *  MyGeo::Vec3f{0.740f+0.287f,0.740f+0.160f,0.740f} + 18.4f * MyGeo::Vec3f{0.737f+0.642f,0.737f+0.159f,0.737f};

    auto lambertian_monolight=genShared<Material,Lambertian>(MyGeo::Vec3f{1,1,1});

    // auto monolight_white=genMonoLight(MyGeo::Vec3f{1,0,1});
    auto monolight_white=genShared<Material,MonoLight>(lightColor);

    // auto sphere100= genSphere(100);
    auto sphere100=genShared<Shape,Sphere>(100);

    // auto cube100=genCube(MyGeo::Vec3f{100,100,200});
    auto cube100=genShared<Shape,Cube>(MyGeo::Vec3f{100,100,200});



    auto wallLeft=genShared<Shape,Rect>(Point{600,0,0},Vect{0,0,555},Vect{0,555,0});
    auto wallRight=genShared<Shape,Rect>(Point{0,0,555},Vect{0,0,-555},Vect{0,555,0});
    auto ceil=genShared<Shape,Rect>(Point{600,555,555},Vect{-600,0,0},Vect{0,0,-555});
    auto floor=genShared<Shape,Rect>(Point{600,0,0},Vect{-600,0,0},Vect{0,0,555});
    auto wallMiddle=genShared<Shape,Rect>(Point{600,0,555},Vect{-600,0,0},Vect{0,555,0});
    auto ceilLight=genShared<Shape,Rect>(Point{360,554.8,325.5},Vect{-120,0,0},Vect{0,0,-100});

    


    // auto smallBox=genShared<Shape,Cube>(MyGeo::Vec3f{165,165,165});
    auto smallBox=genShared<Shape,Cube>(MyGeo::Vec3f{125,125,125});

    auto smallBox_white=genShared<Primitive,Primitive>(smallBox,dielectric);
    // auto samllBox_white_instance=genShared<Instance,Instance>(translateMat({212.5,82.5,147.5})*rotationMat({0,1,0},-18),smallBox_white);
    auto samllBox_white_instance=genShared<Instance,Instance>(translateMat({300,62.5,80})*rotationMat({0,1,0},-28),smallBox_white);


    auto bigBox=genShared<Shape,Cube>(MyGeo::Vec3f{240,380,200});
    auto bigBox_mirror=genShared<Primitive,Primitive>(bigBox,mirror);
    auto bigBox_mirror_instance=genShared<Instance,Instance>(translateMat({380.5,165,377.5})*rotationMat({0,1,0},25),bigBox_mirror);

    auto wallLeft_green=genShared<Primitive,Primitive>(wallLeft,lambertian_green);
    auto wallLeft_white=genShared<Primitive,Primitive>(wallLeft,lambertian_white);

    auto wallRight_red=genShared<Primitive,Primitive>(wallRight,lambertian_red);
    auto ceil_white=genShared<Primitive,Primitive>(ceil,lambertian_white);
    auto floor_white=genShared<Primitive,Primitive>(floor,lambertian_white);
    auto wallMiddle_white=genShared<Primitive,Primitive>(wallMiddle,lambertian_white);
    auto ceilLight_white=genShared<Primitive,Primitive>(ceilLight,monolight_white);


    auto wallLeft_green_instance=genShared<Instance,Instance>(identityMat(),wallLeft_green);
    auto wallLeft_white_instance=genShared<Instance,Instance>(translateMat({-10,0,0})*scaleMat({0.2,0.3,0.4}),wallLeft_white);

    auto wallRight_red_instance=genShared<Instance,Instance>(identityMat(),wallRight_red);
    auto ceil_white_instance=genShared<Instance,Instance>(identityMat(),ceil_white);
    auto floor_white_instance=genShared<Instance,Instance>(identityMat(),floor_white);
    auto wallMiddle_white_instance=genShared<Instance,Instance>(identityMat(),wallMiddle_white);
    auto ceilLight_white_instance=genShared<Instance,Instance>(identityMat(),ceilLight_white);

    auto ball=genShared<Shape,Sphere>(100);
    auto ball_white=genShared<Primitive,Primitive>(ball,lambertian_white);
    // auto ball_white=genShared<Primitive,Primitive>(ball,lambertian_white);

    auto ball_white_instance1=genShared<Instance,Instance>(translateMat({170,140,300})*scaleMat({0.8,1.4,0.5}),ball_white);
    auto ball_white_instance2=genShared<Instance,Instance>(translateMat({300,200,300})*rotationMat({1,1,1},-90)*scaleMat({1.5,0.2,1}),ball_white);



    std::vector<std::shared_ptr<Instance>> lightInstances;
    std::vector<std::shared_ptr<Instance>> instances;

    instances.emplace_back(wallLeft_green_instance);
    // instances.emplace_back(wallLeft_white_instance);
    instances.emplace_back(wallRight_red_instance);
    instances.emplace_back(ceil_white_instance);
    instances.emplace_back(floor_white_instance);
    instances.emplace_back(wallMiddle_white_instance);
    instances.emplace_back(ceilLight_white_instance);
    instances.emplace_back(samllBox_white_instance);
    instances.emplace_back(bigBox_mirror_instance);
    instances.emplace_back(ball_white_instance1);
    // instances.emplace_back(ball_white_instance2);


//get all light instances
    for(auto& p:instances)
    {
        if(p->primitive->material->ifemit()) lightInstances.push_back(p);
    }

    Canvas canvas{800,800};

    auto camera=std::make_shared<PerspectiveCamera>(Point{278,278,-800},Point{278,278,0},Vect{0,1,0},45,(float)canvas.width/canvas.height,Film{MyGeo::Vec2i{canvas.width,canvas.height}},0.f,10.f);
    canvas.setCamera(camera);
    canvas.render(instances,lightInstances,512,16);
}

int main(int argc, char** argv) 
{
    // aggregateTest();return 0;
    renderTest();return 0;
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


