#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Component;
class ENGINE_DLL Object abstract:
    public Base
{
protected:
    Object();
    virtual ~Object() = default;

public:
    virtual HRESULT Initialize();
    virtual void Update(_float dt);
    virtual void LateUpdate(_float dt);
    virtual void Free()override;

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        auto comp = T::Create(std::forward<Args>(args)...);
        components.push_back(comp);
        componentMap[typeid(T)] = comp;
        comp->AddRef();

        return comp;
    }

    template<typename T>
    T* GetComponent()
    {
        auto iter = componentMap.find(typeid(T));
        if (iter != componentMap.end())
            return static_cast<T*>(iter->second);
        else
            return nullptr;
    }
protected:
    std::vector<Component*> components;
    std::unordered_map<std::type_index, Component*> componentMap;
};

NS_END