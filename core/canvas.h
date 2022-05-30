#pragma once
#include"object.h"
#include"camera.h"
#include"../geometry/aggregates.h"
#include"myrandom.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image_write.h>

// #include <stb_image_write.h>


struct Canvas
{
    int width, height;
    std::vector<MyGeo::Vec3c> framebuffer;
    std::shared_ptr<PerspectiveCamera> camera;
    MyGeo::Vec3f backgroundColor{0,0,0};


    Canvas(int width, int height):
    width{width},height{height}
    {
        framebuffer.resize(width*height);
    }

    void setCamera(std::shared_ptr<PerspectiveCamera> cam){camera=cam;}

    void setPixel(int i, int j, const MyGeo::Vec3f& color)
    {
        framebuffer[j*width+i]=
        MyGeo::Vec3c{(uint8_t)(255*clamp(0.f,0.999f,color.x)),
        (uint8_t)(255*clamp(0.f,0.999f,color.y)),
        (uint8_t)(255*clamp(0.f,0.999f,color.z))};

        // std::cout<<(int)framebuffer[j*width+i].x<<(int)framebuffer[j*width+i].y<<(int)framebuffer[j*width+i].z<<std::endl;
    }

    void render(const std::vector<std::shared_ptr<Instance>>& instances ,int spp, int maxdepth)
    {
        for(int j=0;j<height;++j)
        {
            // std::cerr<<j<<std::endl;
            for(int i=0;i<width;++i)
            {
                MyGeo::Vec3f pixelColor{0,0,0};
                for(int k=0;k<spp;++k)
                {
                    auto ray=camera->genRay(i,j);
                    // std::cout<<i<<','<<j<<" : "<<ray;
                    pixelColor+=trace(ray,instances,maxdepth);
                }
                pixelColor/=spp;
                for(auto& x:pixelColor.data)
                {
                    x=std::fabs(x);
                }
                // std::cout<<"color "<<i<<' '<<j<<' '<<std::endl;//<<pixelColor<<std::endl;
                setPixel(i,j,pixelColor);
            }
        }
        std::cout<<"write png"<<std::endl;
        stbi_write_png("test.png",width,height,3,framebuffer.data(),3*width);
    }

float lightArea=0;


    void sampleLight(const std::vector<std::shared_ptr<Instance>>& lights,HitRecord& sampleRec)
    {
        for(auto const& light: lights)
        {
            lightArea+=light->area();
        }
        float threshold=getRand(0.f,1.f)*lightArea;
    }


    MyGeo::Vec3f trace(const Ray& ray, const std::vector<std::shared_ptr<Instance>>& instances , int maxdepth)
    {
        HitRecord rec;
        bool hitflag=false;
        for(auto const& instance:instances)
        {
            if(instance->hit(ray,rec))
            {
                hitflag=true;
            }
        }
        if(!hitflag) return backgroundColor;

        if(rec.material->ifemit())
        {
            return rec.material->getColor(rec.u,rec.v);
        }
        

        auto sampleDir=randomInHemisphere(rec.normal.v3);
        // std::cout<<sampleDir<<std::endl; exit(0);
        // return shade(rec,-ray.direction.v3,sampleDir);

static Point lightPos{277.5,277.5,277.5};

auto lightDir=(lightPos-rec.position);
auto d2=lightDir.v3.norm2();
auto damp=1.f/d2*200*200;
lightDir.normalize();



        // return rec.material->getColor(rec.u,rec.v);
        return damp*clamp(0.f,1.f,rec.normal.v3.normalize().dot(lightDir.v3))*rec.material->getColor(rec.u,rec.v);
        
        // return std::fabs(rec.normal.v3.normalize().dot(lightDir.v3))*rec.material->getColor(rec.u,rec.v);

        // return {0.f,1.f,0.f};
    }


    MyGeo::Vec3f traceBHV(const Ray& ray, float t0, float t1, const BVH& bvh, int maxdepth)
    {
        HitRecord rec;
        if(!bvh.hit(ray,rec)) return backgroundColor;

        if(rec.material->ifemit())
        {
            return rec.material->getColor(rec.u,rec.v);
        }

        return backgroundColor;
    }

    //shade surface point
    //sample a light, then trace on
    MyGeo::Vec3f shade(const HitRecord& rec, const MyGeo::Vec3f& lo, const MyGeo::Vec3f& li) const
    {
        //Sample

        
    }

    // void sampleLight(const std::vector<std::shared_ptr<Instance>>& instances, HitRecord& sampleRec)
    // {
    //     float sumArea=0.f;
        
    //     for(auto instance:instances)
    //     {
            
    //         if(instance->primitive->material->emitColor(0,0))
    //         {
    //             sumArea+=instance->primitive->shape->area();
    //         }
    //     }
    //     float threshold=getRand(0.f,1.f)*lightArea;
    //     // std::cout<<"threashold "<<threshold<<std::endl;
    //     float accum=0.f;
    //     for(auto obj:instances)
    //     {
    //         if(obj->ifemit())
    //         {
    //             accum+=obj->area().value();
    //             // std::cout<<"accum "<<accum<<std::endl;
    //             if (accum>threshold)
    //             {
    //                 obj->sample(sampleRec);
    //                 return;
    //             }
    //         }
    //     }
    // }

     
};
