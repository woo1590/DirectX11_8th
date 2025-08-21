#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Component;
class ENGINE_DLL Object abstract:
    public Base
{
protected:
    Object();
    Object(const Object& prototype);
    virtual ~Object() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* arg);
    virtual void Update(_float dt);
    virtual void LateUpdate(_float dt);

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

    virtual Object* Clone(void* arg) = 0;
    virtual void Free()override;
protected:
    std::vector<Component*> components;
    std::unordered_map<std::type_index, Component*> componentMap;

};

NS_END