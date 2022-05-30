#pragma once
#include<mygeo/vec.h>
#include"../geometry/utils.h"
#include<vector>
#include"colorConstants.h"
// typedef RGBSpectrum Spectrum;
// typedef SampledSpectrum Spectrum;
enum class SpectrumType { Reflectance, Illuminant };
static const int sampledLambdaStart = 400;
static const int sampledLambdaEnd = 700;
static const int nSpectrumSamples = 60;

static float interpolateSpectrumSamples
(const float *lambda, const float *vals, int n, float k) 
{
    if (k <= lambda[0]) return vals[0];
    if (k >= lambda[n - 1]) return vals[n - 1];
    int offset = findInterval(n, [&](int index) { return lambda[index] <= k; });\
    float t = (k - lambda[offset]) / (lambda[offset + 1] - lambda[offset]);
    return lerp(t, vals[offset], vals[offset + 1]);
}
inline void XYZToRGB(const float xyz[3], float rgb[3]) 
{
    rgb[0] =  3.240479f*xyz[0] - 1.537150f*xyz[1] - 0.498535f*xyz[2];
    rgb[1] = -0.969256f*xyz[0] + 1.875991f*xyz[1] + 0.041556f*xyz[2];
    rgb[2] =  0.055648f*xyz[0] - 0.204043f*xyz[1] + 1.057311f*xyz[2];
}
inline void RGBToXYZ(const float rgb[3], float xyz[3]) 
{
    xyz[0] = 0.412453f*rgb[0] + 0.357580f*rgb[1] + 0.180423f*rgb[2];
    xyz[1] = 0.212671f*rgb[0] + 0.715160f*rgb[1] + 0.072169f*rgb[2];
    xyz[2] = 0.019334f*rgb[0] + 0.119193f*rgb[1] + 0.950227f*rgb[2];
}


static bool SpectrumSamplesSorted(const float *lambda, const float *vals, int n) 
{
    for (int i = 0; i < n - 1; ++i)
        if (lambda[i] > lambda[i + 1]) return false;
    return true;
}

void SortSpectrumSamples(float *lambda, float *vals, int n) 
{
    std::vector<std::pair<float, float>> sortVec;
    sortVec.reserve(n);
    for (int i = 0; i < n; ++i)
        sortVec.push_back(std::make_pair(lambda[i], vals[i]));
    std::sort(sortVec.begin(), sortVec.end());
    for (int i = 0; i < n; ++i) 
    {
        lambda[i] = sortVec[i].first;
        vals[i] = sortVec[i].second;
    }
}
float averageSpectrumSamples(const float *lambda, const float *vals, int n, float lambdaStart,float lambdaEnd) 
{
    // Handle cases with out-of-bounds range or single sample only
    if (lambdaEnd <= lambda[0]) return vals[0];
    if (lambdaStart >= lambda[n - 1]) return vals[n - 1];
    if (n == 1) return vals[0];
    float sum = 0;
    // Add contributions of constant segments before/after samples
    if (lambdaStart < lambda[0]) sum += vals[0] * (lambda[0] - lambdaStart);
    if (lambdaEnd > lambda[n - 1])
        sum += vals[n - 1] * (lambdaEnd - lambda[n - 1]);

    // Advance to first relevant wavelength segment
    int i = 0;
    while (lambdaStart > lambda[i + 1]) ++i;

    // Loop over wavelength sample segments and add contributions
    auto interp = [lambda, vals](float w, int i) {
        return lerp((w - lambda[i]) / (lambda[i + 1] - lambda[i]), vals[i],
                    vals[i + 1]);
    };
    for (; i + 1 < n && lambdaEnd >= lambda[i]; ++i) {
        float segLambdaStart = std::max(lambdaStart, lambda[i]);
        float segLambdaEnd = std::min(lambdaEnd, lambda[i + 1]);
        sum += 0.5 * (interp(segLambdaStart, i) + interp(segLambdaEnd, i)) *
               (segLambdaEnd - segLambdaStart);
    }
    return sum / (lambdaEnd - lambdaStart);
}

template<int N> 
struct CoefficientSpectrum
{
    static const int sampleNum=N;

    MyGeo::Vec<float,N> color;
    CoefficientSpectrum(float v = 0.f) 
    { 
        for (int i = 0; i < nSpectrumSamples; ++i)
            color[i] = v;
    }
    bool isDark() const 
    {
        for(auto const& c:color)
        {
            if(c!=0.f) return false; 
        }
        return true;
    }
};



struct SampledSpectrum: public CoefficientSpectrum<nSpectrumSamples>
{
    static SampledSpectrum X, Y, Z;

    static SampledSpectrum fromSampled(const float* lambda, const float* v, int n)
    {
        if(!SpectrumSamplesSorted(lambda,v,n))
        {
            std::vector<float> slambda(&lambda[0],&lambda[n]);
            std::vector<float> sv(&v[0],&v[n]);
            SortSpectrumSamples(&slambda[0],&sv[0],n);
            return fromSampled(&slambda[0],&sv[0],n);
        }
        SampledSpectrum r;
        for(int i=0;i<sampleNum;++i)
        {
            float lambda0=lerp((float)i/nSpectrumSamples,sampledLambdaStart,sampledLambdaEnd);
            float lambda1=lerp((float)(i+1)/nSpectrumSamples,sampledLambdaStart,sampledLambdaEnd);
            
            r.color[i]=averageSpectrumSamples(lambda,v,n,lambda0,lambda1);

        }
        return r;
    }

    static void init()
    {
        for(int i=0;i<nSpectrumSamples;++i)
        {
            float wl0=lerp((float)i/float(nSpectrumSamples),sampledLambdaStart,sampledLambdaEnd);
            float wl1=lerp((float)(i+1)/float(nSpectrumSamples),sampledLambdaStart,sampledLambdaEnd);
            X.color[i] = averageSpectrumSamples(CIE_lambda, CIE_X, nCIESamples,
                                            wl0, wl1);
            Y.color[i] = averageSpectrumSamples(CIE_lambda, CIE_Y, nCIESamples,
                                            wl0, wl1);
            Z.color[i] = averageSpectrumSamples(CIE_lambda, CIE_Z, nCIESamples,
                                            wl0, wl1);
        }
    }

    void toXYZ(float xyz[3]) const 
    {
        for(int i=0;i<nSpectrumSamples;++i)
        {
            xyz[0]+=X.color[i]*color[i];
            xyz[1]+=Y.color[i]*color[i];
            xyz[2]+=Z.color[i]*color[i];
        }
        float scale=(float)(sampledLambdaEnd-sampledLambdaStart)/(CIE_Y_integral*nSpectrumSamples);
        xyz[0]*=scale;
        xyz[1]*=scale;
        xyz[2]*=scale;
    }
    //luminance
    float y() const 
    {
        float y=0;
        for(int i=0;i<nSpectrumSamples;++i)
        {
            y+=Y.color[i]*color[i];
            return y*(float)(sampledLambdaEnd-sampledLambdaStart)/nSpectrumSamples;
        }
    }


};





struct RGBSpectrum: public CoefficientSpectrum<3>
{
    RGBSpectrum(float v=0.f) :CoefficientSpectrum<3>(v){}
    RGBSpectrum(const CoefficientSpectrum<3>& v) :CoefficientSpectrum<3>(v){}
    RGBSpectrum(const RGBSpectrum& s,SpectrumType type=SpectrumType::Reflectance)
    {
        *this=s;
    }

    static RGBSpectrum fromRGB(const float rgb[3],SpectrumType type=SpectrumType::Reflectance)
    {
        RGBSpectrum s;
        s.color[0]=rgb[0];
        s.color[1]=rgb[1];
        s.color[2]=rgb[2];
        return s;
    }
    // void toRGB(float* rgb) const 
    // {
    //     rgb[0]=color[0];
    //     rgb[1]=color[1];
    //     rgb[2]=color[2];
    // }
    // const RGBSpectrum &toRGBSpectrum() const 
    // {
    //     return *this;
    // }
    void toXYZ(float xyz[3]) const 
    { 
        RGBToXYZ(color.data.data(), xyz);
    }

    static RGBSpectrum fromXYZ(const float xyz[3], SpectrumType type=SpectrumType::Reflectance) 
    {
        RGBSpectrum r;
        XYZToRGB(xyz, r.color.data.data());
        return r;
    }
    float y() const 
    {
        const float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
        return YWeight[0] * color[0] + YWeight[1] * color[1] + YWeight[2] * color[2];
    }
    static RGBSpectrum FromSampled(const float *lambda, const float *v,
                                    int n)
    {
       if (!SpectrumSamplesSorted(lambda, v, n)) 
       {
           std::vector<float> slambda(&lambda[0], &lambda[n]);
           std::vector<float> sv(&v[0], &v[n]);
           SortSpectrumSamples(&slambda[0], &sv[0], n);
           return FromSampled(&slambda[0], &sv[0], n);
       }
        float xyz[3] = { 0, 0, 0 };
        for (int i = 0; i < nCIESamples; ++i) 
        {
            float val = interpolateSpectrumSamples(lambda, v, n,
                                                    CIE_lambda[i]);
            xyz[0] += val * CIE_X[i];
            xyz[1] += val * CIE_Y[i];
            xyz[2] += val * CIE_Z[i];
        }
        float scale = float(CIE_lambda[nCIESamples-1] - CIE_lambda[0]) /
            float(CIE_Y_integral * nCIESamples);
        xyz[0] *= scale;
        xyz[1] *= scale;
        xyz[2] *= scale;
        return fromXYZ(xyz);    
    }
};
