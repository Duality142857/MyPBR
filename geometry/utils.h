#pragma once
#include<optional>

static constexpr float Pi=3.14159265f;
static constexpr float ReciPi=1.f/Pi;

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
template <typename T, typename U, typename V>
inline T clamp(T low, U high, V val) 
{
    if (val < low)
        return low;
    else if (val > high)
        return high;
    else
        return val;
}

template<class T>
T lerp(float t, const T& x0, const T& x1)
{
    return t*x0+(1-t)*x1;
}

template <typename Predicate>
int findInterval(int size, const Predicate &pred) 
{
    int first = 0, len = size;
    while (len > 0) 
    {
        int half = len >> 1, middle = first + half;
        // Bisect range based on value of _pred_ at _middle_
        if (pred(middle)) 
        {
            first = middle + 1;
            len -= half + 1;
        } 
        else
            len = half;
    }
    
    return clamp(first - 1, 0, size - 2);
}