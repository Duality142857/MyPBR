#pragma once
#include"object.h"
#include"camera.h"
#include"../geometry/aggregates.h"
#include"myrandom.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image_write.h>
#include<fstream>

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

    void render(const std::vector<std::shared_ptr<Instance>>& instances,const std::vector<std::shared_ptr<Instance>>& lights ,int spp, int maxdepth)
    {
        for(int j=0;j<height;++j)
        {
            std::cerr<<"\rFinished "<<j<<" lines"<<std::flush;
#pragma omp parallel for num_threads(8)
            for(int i=0;i<width;++i)
            {
                MyGeo::Vec3f pixelColor{0,0,0};
                for(int k=0;k<spp;++k)
                {
                    auto ray=camera->genRay(i,j);
                    pixelColor+=trace(ray,instances,lights);
                }
                pixelColor/=spp;
                for(auto& x:pixelColor.data)
                {
                    x=std::fabs(x);
                }
                setPixel(i,j,pixelColor);
            }
        }
        std::cout<<"write png"<<std::endl;
        stbi_write_png("test.png",width,height,3,framebuffer.data(),3*width);
    }

float lightArea=0;

    MyGeo::Vec3f trace(const Ray& ray, const std::vector<std::shared_ptr<Instance>>& instances,const std::vector<std::shared_ptr<Instance>>& lights)
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
        // rec.fixNormal(ray.direction.v3);

        if(rec.material->ifemit())
        {
            return rec.material->getColor(rec.u,rec.v);
        }
        return shade(instances,lights,-ray.direction.v3,rec,1);
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
    //lo: direction of tracing ray, li will be generated later
    MyGeo::Vec3f shade(const std::vector<std::shared_ptr<Instance>>& instances,const std::vector<std::shared_ptr<Instance>>& lights, const MyGeo::Vec3f& wo, HitRecord& rec,int depth)
    {
        //Sample light
        if(depth>64) return {0,0,0};
        HitRecord sampleRec;
        sampleLight(lights,sampleRec);
        HitRecord checkrec;
        auto dvec=sampleRec.position.v3-rec.position.v3;
        auto d2_reci=1.f/dvec.norm2();
        auto dvec_normalized=dvec.normalVec();
        float cos0=dvec_normalized.dot(rec.normal.v3);
        float cos1=-dvec_normalized.dot(sampleRec.normal.v3);
        Ray ray{rec.position,dvec_normalized};
        for(auto const& instance:instances)
        {
            instance->hit(ray,checkrec);
        }

        auto checkvec=(checkrec.position-rec.position).v3;
        auto checkd2=checkvec.norm2();

        MyGeo::Vec3f L_direct{0,0,0};

        if(checkd2>=dvec.norm2()-1 && checkvec.dot(checkrec.normal.v3)<=0 ) 
        {
            L_direct=rec.material->brdf(wo,dvec_normalized)*sampleRec.material->getColor(rec.u,rec.v)*cos0*cos1*d2_reci*sampleRec.area;
        }


        static const float rus=0.8;
        if(getRand(0.01f,0.99f)>rus) return L_direct;

        // rec.fixNormal(-wo);

        auto wi=randomInHemisphere(rec.normal.v3);
        assert(wi.dot(rec.normal.v3)>=0);
        HitRecord secondRec;
        Ray secondRay{rec.position,wi};
        bool secondHitFlag=false;
        for(auto const& instance:instances)
        {
            if(instance->hit(secondRay,secondRec)) secondHitFlag=true;
        }
        if(!secondHitFlag || secondRec.material->ifemit()) return L_direct;

        float cos=wi.dot(rec.normal.v3);
        MyGeo::Vec3f L_indirect=rec.material->brdf(wo,wi)*shade(instances,lights,-wi,secondRec,depth+1)*cos*2*Pi*1.25f;
        return L_direct+L_indirect;
        
    }

float sumArea=-1.f;
void computeLightsArea(const std::vector<std::shared_ptr<Instance>>& lights)
{
    sumArea=0.f;
    for(auto& light:lights)
    {
        sumArea+=light->area();
    }
}

    void sampleLight(const std::vector<std::shared_ptr<Instance>>& lights, HitRecord& sampleRec)
    {
        if(sumArea<0) computeLightsArea(lights);
        float threshold=clamp(0.f,sumArea,getRand(0.f,1.f)*sumArea);
        float accum=0.f;
        for(auto& light:lights)
        {
            accum+=light->area();
            if(accum>=threshold)
            {
                light->sample(sampleRec);
                return;
            }
            std::cout<<"asdf"<<std::endl;
        }
    }
     
};
