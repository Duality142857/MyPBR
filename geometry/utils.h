#pragma once
#include<optional>

static constexpr float Pi=3.14159265f;

static constexpr float toRad(float x)
{
    return x*3.1415927/180.0;
}

static std::optional<std::tuple<float,float>> solveQuadratic(const float& a, const float& b, const float& c)
{
    float d=b*b-4*a*c;
    if(d<0) return std::nullopt;
    float recidenom=0.5f/a;
    float m=-b*recidenom;
    float n=std::sqrt(d)*recidenom;
    float x1=m-n,x2=m+n;
    if(x1>x2) std::swap(x1,x2);
    return std::make_optional(std::make_tuple(x1,x2));
}