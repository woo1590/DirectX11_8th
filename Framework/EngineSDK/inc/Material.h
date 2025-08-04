#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Material :
    public Base
{
private:
    Material();
    virtual ~Material() = default;

public:
    static Material* Create();
    HRESULT Initialize();
    void Free()override;

    void SetParam(const std::string& key, _int value);
    void SetParam(const std::string& key, _float value);

private:
    std::unordered_map<std::string, _int> intParams;
    std::unordered_map<std::string, _float> floatParams;
};

NS_END