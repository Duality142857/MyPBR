#pragma once
#include"object.h"
#include"camera.h"




struct Canvas
{
    int width, height;
    std::vector<MyGeo::Vec3c> framebuffer;
    PerspectiveCamera camera;
    MyGeo::Vec3f backgroundColor{0,0,0};


    Canvas(int width, int height):
    width:{width},height{height}
    {}

    void setPixel(int i, int j, const MyGeo::Vec3f& color)
    {
        framebuffer[j*width+i]=
        MyGeo::Vec3c{(uint8_t)(255*clamp(0.f,0.999f,color.x)),
        (uint8_t)(255*clamp(0.f,0.999f,color.y)),
        (uint8_t)(255*clamp(0.f,0.999f,color.z))};
    }

     
};
