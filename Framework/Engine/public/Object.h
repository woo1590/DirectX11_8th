#pragma once
#include "Base.h"
#include "EngineCore.h"
#include "RenderProxy.h"
#include "TransformComponent.h"

NS_BEGIN(Engine)

class Component;
class TransformComponent;
class ENGINE_DLL Object abstract:
    public Base
{
public:
    typedef struct ObjectDesc : public TransformComponent::TRANSFORM_DESC
    {
        _string instanceTag{};
    }OBJECT_DESC;

protected:
    Object();
    Object(const Object& prototype);
    virtual ~Object() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(InitDESC* arg);
    virtual void PriorityUpdate(_float dt);
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
    T* GetComponent()
    {
        auto iter = m_ComponentMap.find(typeid(T));
        if (iter != m_ComponentMap.end())
            return static_cast<T*>(iter->second);
        else
            return nullptr;
    }

    virtual HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);
    _string GetInstanceTag()const { return m_strInstanceTag; }

    virtual Object* Clone(InitDESC* arg) = 0;
    virtual void Free()override;

#ifdef USE_IMGUI
    void RenderInspector();
#endif

protected:
    _string m_strInstanceTag{};
    static _uint m_iInstanceID;
    TransformComponent* m_pTransform = nullptr;

    std::vector<Component*> m_Components;
    std::unordered_map<std::type_index, Component*> m_ComponentMap;
};

NS_END