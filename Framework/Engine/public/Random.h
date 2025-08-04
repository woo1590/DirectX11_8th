#pragma once
#include "Base.h"

NS_BEGIN(Engine)    

class Random :
    public Base
{
private:
    Random();
    virtual ~Random() = default;

public:
    static Random* Create();

    template<typename T>
    std::enable_if_t<std::is_integral_v<T>, T>
        get(T min, T max)
    {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(rng);
    }

    template<typename T>
    std::enable_if_t<std::is_floating_point_v<T>, T>
        get(T min, T max)
    {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(rng);
    }

private:
    void Free()override {}

    std::mt19937 rng;

};

NS_END