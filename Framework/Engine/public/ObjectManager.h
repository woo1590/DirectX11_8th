#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Layer;
class ObjectManager :
    public Base
{
private:
    ObjectManager();
    virtual ~ObjectManager() = default;

public:
    static ObjectManager* Create();
    HRESULT Initialize();
    void Free()override;

private:
    std::unordered_map<std::string, Layer*> layers;

};

NS_END