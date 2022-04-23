#pragma once
#include<mygeo/vec.h>
#include"ray.h"
#include"transforms.h"
#include"bbox.h"


struct Film
{
    MyGeo::Vec2i resolution;
};

struct Camera
{
    virtual Ray genRay(float x, float y) const =0;

};

struct PerspectiveCamera : public Camera
{
    float fov;
    float aspectRatio;
    float lensRadius;
    float focalDistance;
    Point position;
    Vect zaxis,xaxis,yaxis;


    MatrixTransform cam2world;
    MatrixTransform world2cam;
    MatrixTransform cam2scr;
    MatrixTransform scr2rst;
    MatrixTransform rst2scr;
    MatrixTransform rst2cam;


    PerspectiveCamera(
        Point position,
        Point lookat,
        Vect up,
        float fov,
        float aspectRatio,
        // const MatrixTransform& cam2world, 
        // const MatrixTransform& cam2scr,
        const BB2f& screen,
        const Film& film,
        float lensRadius,
        float focalDistance
        ):
        position{position},
        fov{fov},
        aspectRatio{aspectRatio},
        lensRadius{lensRadius},
        focalDistance{focalDistance},
        zaxis{(position-lookat).normalize()},
        xaxis{up.cross(zaxis).normalize()},
        yaxis{zaxis.cross(xaxis)},
        cam2world{
            MatrixTransform{
                translateMat(position.v3)*
                MyGeo::Mat4f{
                    xaxis.v4,yaxis.v4,zaxis.v4,{0,0,0,1}
                }
            }
        },
        world2cam{MatrixTransform{
            MyGeo::Mat4f{
            MyGeo::Vec4f{xaxis.x,yaxis.x,zaxis.x,0},
            MyGeo::Vec4f{xaxis.y,yaxis.y,zaxis.y,0},
            MyGeo::Vec4f{xaxis.z,yaxis.z,zaxis.z,0},
            MyGeo::Vec4f{0,0,0,1}
            }
        }*translateMat(-position.v3)},
        //cam2scr == perspective transform
        cam2scr{getPerspectiveTransform(fov,-1e-2f,-1000.f)},
        scr2rst{
            scaleMat(MyGeo::Vec3f{(float)film.resolution.x,(float)film.resolution.y,1.f})
            *scaleMat(MyGeo::Vec3f{1.f/(screen.max.x-screen.min.x),1.f/(screen.min.y-screen.max.y),1.f})
            *translateMat(MyGeo::Vec3f{-screen.min.x,-screen.max.y,0.f})
            },
        rst2scr{scr2rst.inverse()},
        rst2cam{cam2scr.inverse()*rst2scr}
        {
            // std::cout<<" cam2scr: "<<std::endl;
            // std::cout<<cam2scr.m<<std::endl;
            // std::cout<<" scr2rst "<<std::endl;
            // std::cout<<scr2rst.m<<std::endl;
            // std::cout<<" rst2scr "<<std::endl;
            // std::cout<<rst2scr.m<<std::endl;

            std::cout<<" rst2cam "<<std::endl;
            std::cout<<rst2cam.m<<std::endl;
        }

    Ray genRay(float x, float y) const override
    {
        // std::cout<<"asdf "<<cam2scr(Point{-1,1,-100}).v3<<std::endl;
        Point filmPoint{x,y,0.f};
        std::cout<<rst2cam.m*filmPoint.v4<<std::endl;
        // std::cout<<"scrpoint "<<rst2scr(filmPoint).v3<<std::endl;
        std::cout<<"campoint "<<rst2cam(filmPoint).v3<<std::endl;


        return cam2world(Ray{{0,0,0},Vect{rst2cam(filmPoint).v3}.normalize()});
    }     

    MatrixTransform getPerspectiveTransform(float fov, float n, float f) const 
    {
        MyGeo::Mat4f per{
            MyGeo::Vec4f{-1,0,0,0},
            {0,-1,0,0},
            {0,0,f/(f-n),1},
            {0,0,f*n/(n-f),0}
        };

        float reciTan=1.f/std::tan(toRad(fov)/2);
        float scaleFactor=reciTan*focalDistance;
        // std::cout<<(scaleMat({reciTan,reciTan,1.f})*MatrixTransform{per}).m;
        return scaleMat({scaleFactor,scaleFactor,1.f})*MatrixTransform{per};
    }

};
