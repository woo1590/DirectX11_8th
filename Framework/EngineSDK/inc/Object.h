#pragma once
#include "Base.h"
#include "EngineCore.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Component;
class TransformComponent;
class ENGINE_DLL Object abstract:
    public Base
{
public:
    struct ObjectDESC : public InitDESC
    {

    };

protected:
    Object();
    Object(const Object& prototype);
    virtual ~Object() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(InitDESC* arg);
    virtual void Update(_float dt);
    virtual void LateUpdate(_float dt);

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        auto comp = T::Create(this,std::forward<Args>(args)...);
        m_Components.push_back(comp);
        m_ComponentMap[typeid(T)] = comp;
        comp->AddRef();

        return comp;
    }

    template<typename T>
    T* AddComponentFromPrototype(_uint prototypeLevel, const _string& prototypeTag, InitDESC* arg = nullptr)
    {
        Base* clone = EngineCore::GetInstance()->ClonePrototype(prototypeLevel, prototypeTag, arg);
        if (!clone)
            return nullptr;

        T* comp = dynamic_cast<T*>(clone);
        if (!comp)
            return nullptr;

        m_Components.push_back(comp);
        m_ComponentMap[typeid(T)] = comp;
        comp->SetOwner(this);
        comp->AddRef();

        return comp;
    }

    template<typename T>
    T* GetComponent()
    {
        auto iter = m_ComponentMap.find(typeid(T));
        if (iter != m_ComponentMap.end())
            return static_cast<T*>(iter->second);
        else
            return nullptr;
    }

    virtual HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies) { return S_OK; }
    virtual Object* Clone(InitDESC* arg) = 0;
    virtual void Free()override;

protected:
    TransformComponent* m_pTransform = nullptr;

    std::vector<Component*> m_Components;
    std::unordered_map<std::type_index, Component*> m_ComponentMap;
};

NS_END