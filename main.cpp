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
    PerspectiveCamera cam{Point{0,0,-1},Point{0,0,0},Vect{0,1,0},45,1.f,Film{MyGeo::Vec2i{800,800}},0.f,1.f};
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
    std::shared_ptr<Material> lam=std::make_shared<Lambertian>(MyGeo::Vec3f{1,1,1});
    std::shared_ptr<Shape> sphere= std::make_shared<Sphere>(200);
    auto primitive=std::make_shared<Primitive>(sphere, lam);
    Instance instance{rotationMat({0,0,1},0),primitive};
    std::cout<<instance.bound();
}



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


    // auto lambertian_white=genLambertian(MyGeo::Vec3f{1,1,1});
    // auto lambertian_green=genLambertian(MyGeo::Vec3f{0,1,0});
    // auto lambertian_red=genLambertian(MyGeo::Vec3f{1,0,0});
    // auto lambertian_blue=genLambertian(MyGeo::Vec3f{0,0,1});

    auto lambertian_white=genShared<Material,Lambertian>(MyGeo::Vec3f{1,1,1});
    auto lambertian_green=genShared<Material,Lambertian>(MyGeo::Vec3f{0,1,0});
    auto lambertian_red=genShared<Material,Lambertian>(MyGeo::Vec3f{1,0,0});
    auto lambertian_blue=genShared<Material,Lambertian>(MyGeo::Vec3f{0,0,1});

    // auto monolight_white=genMonoLight(MyGeo::Vec3f{1,0,1});
    auto monolight_white=genShared<Material,MonoLight>(MyGeo::Vec3f{1,0,1});

    // auto sphere100= genSphere(100);
    auto sphere100=genShared<Shape,Sphere>(100);

    // auto cube100=genCube(MyGeo::Vec3f{100,100,200});
    auto cube100=genShared<Shape,Cube>(MyGeo::Vec3f{100,100,200});



    auto wallLeft=genShared<Shape,Rect>(Point{555,0,0},Vect{0,0,555},Vect{0,555,0});
    auto wallRight=genShared<Shape,Rect>(Point{0,0,555},Vect{0,0,-555},Vect{0,555,0});
    auto ceil=genShared<Shape,Rect>(Point{555,555,555},Vect{-555,0,0},Vect{0,0,-555});
    auto floor=genShared<Shape,Rect>(Point{555,0,0},Vect{-555,0,0},Vect{0,0,555});
    auto wallMiddle=genShared<Shape,Rect>(Point{555,0,555},Vect{-555,0,0},Vect{0,555,0});


    auto wallLeft_green=genShared<Primitive,Primitive>(wallLeft,lambertian_green);
    auto wallRight_red=genShared<Primitive,Primitive>(wallRight,lambertian_red);
    auto ceil_white=genShared<Primitive,Primitive>(ceil,lambertian_white);
    auto floor_white=genShared<Primitive,Primitive>(floor,lambertian_white);
    auto wallMiddle_white=genShared<Primitive,Primitive>(wallMiddle,lambertian_white);

    auto wallLeft_green_instance=genShared<Instance,Instance>(identityMat(),wallLeft_green);


    auto wallRight_red_instance=genShared<Instance,Instance>(identityMat(),wallRight_red);
    auto ceil_white_instance=genShared<Instance,Instance>(identityMat(),ceil_white);
    auto floor_white_instance=genShared<Instance,Instance>(identityMat(),floor_white);
    auto wallMiddle_white_instance=genShared<Instance,Instance>(identityMat(),wallMiddle_white);







//     auto cube100_lambertian_green_primitive=std::make_shared<Primitive>(cube100,lambertian_green);
    
//     auto sphere100_lambertian_white_primitive=std::make_shared<Primitive>(sphere100,lambertian_green);
//     auto sphere100_monolight_white_primitive=std::make_shared<Primitive>(sphere100,monolight_white);

//     auto sphere100_lambertian_white_instance1=std::make_shared<Instance>(translateMat({278,278,0})*scaleMat({1,2,3}),sphere100_lambertian_white_primitive);

//     auto cube100_lambertian_green_instance1=std::make_shared<Instance>(translateMat({278,278,0})*rotationMat({1,0,0},55)*rotationMat({0,1,0},35),cube100_lambertian_green_primitive);

//     auto rect=std::make_shared<Rect>(Point{555,0,0},Vect{0,0,555},Vect{0,555,0});
//     auto rect_lambertian_green_primitive=std::make_shared<Primitive>(rect,lambertian_green);
//     MyGeo::Vec3f mvec{555,275.5,275.5};
    
//     auto rect_lambertian_green_instance=std::make_shared<Instance>(translateMat(mvec)*rotationMat({1,1,1},40)*rotationMat({1,0,1},60)*translateMat(-mvec),rect_lambertian_green_primitive);

//     auto sphere100_lambertian_white_instance2=std::make_shared<Instance>(translateMat({444,444,0}),sphere100_lambertian_white_primitive);

//     auto monolight_sphere100_white_instance1=std::make_shared<Instance>(translateMat({200,200,-50})*rotationMat({1,1,1},45)*scaleMat({1,2,3}),sphere100_monolight_white_primitive);

// //sample test
//     HitRecord sampleRec;
//     monolight_sphere100_white_instance1->sample(sampleRec);//exit(0);


    std::vector<std::shared_ptr<Instance>> lightInstances;
    // lightInstances.push_back(monolight_sphere100_white_instance1);



    std::vector<std::shared_ptr<Instance>> instances;

    instances.emplace_back(wallLeft_green_instance);
    instances.emplace_back(wallRight_red_instance);
    instances.emplace_back(ceil_white_instance);
    instances.emplace_back(floor_white_instance);
    instances.emplace_back(wallMiddle_white_instance);


    // instances.push_back(sphere100_lambertian_white_instance1);
    // instances.push_back(sphere100_lambertian_white_instance2);
    // instances.push_back(monolight_sphere100_white_instance1);
    // instances.push_back(rect_lambertian_green_instance);
    // instances.push_back(cube100_lambertian_green_instance1);

//get all light instances
    for(auto& p:instances)
    {
        if(p->primitive->material->ifemit()) lightInstances.push_back(p);
    }



    Canvas canvas{800,800};

    auto camera=std::make_shared<PerspectiveCamera>(Point{278,278,-800},Point{278,278,0},Vect{0,1,0},45,(float)canvas.width/canvas.height,Film{MyGeo::Vec2i{canvas.width,canvas.height}},0.f,10.f);
    canvas.setCamera(camera);
    canvas.render(instances,1,16);
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


