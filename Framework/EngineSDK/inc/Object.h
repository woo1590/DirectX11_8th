#pragma once
#include "Base.h"
#include "EngineCore.h"
#include "RenderProxy.h"
#include "TransformComponent.h"
#include "EventHandler.h"

NS_BEGIN(Engine)

class Component;
class ColliderComponent;
class TransformComponent;
class ENGINE_DLL Object abstract:
    public Base,
    public EventHandler
{
public:
    typedef struct ObjectDesc : public TransformComponent::TRANSFORM_DESC
    {
        _string instanceTag{};
        _string modelTag{};
    }OBJECT_DESC;

protected:
    Object();
    Object(const Object& prototype);
    virtual ~Object() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(InitDESC* arg);
    virtual HRESULT LateInitialize() { return S_OK; }
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

    virtual void SetDead() { m_isDead = true; }
    _bool IsDead()const { return m_isDead; }
    void SetRenderGroup(RenderGroup group) { m_eRenderGroup = group; }

    /*collision*/
    virtual void OnCollisionEnter(ColliderComponent* otherCollider) {};
    virtual void OnCollisionStay(ColliderComponent* otherCollider) {};
    virtual void OnCollisionExit(ColliderComponent* otherCollider) {};

    virtual Object* Clone(InitDESC* arg) = 0;
    virtual void Free()override;

#ifdef USE_IMGUI
    virtual void RenderInspector();
#endif

protected:
    _bool m_IsLateInitialized = false;
    _string m_strInstanceTag{};
    static _uint m_iInstanceID;
    _uint m_iCurrLevelID{};
    TransformComponent* m_pTransform = nullptr;

    std::vector<Component*> m_Components;
    std::unordered_map<std::type_index, Component*> m_ComponentMap;

    RenderGroup m_eRenderGroup = RenderGroup::NonBlend;
    _bool m_isDead = false;

protected:
    /*fsm*/
    class State
    {
    public:
        virtual void Enter(Object* object) = 0;
        virtual void Update(Object* object, _float dt) = 0;
        virtual void TestForExit(Object* object) = 0;
    };

    class StateNothing :public State
    {
    public:
        void Enter(Object* object)override {};
        void Update(Object* object, _float dt)override {};
        void TestForExit(Object* object)override {};
    };

    StateNothing m_StateNothing;
    State* m_CurrState = &m_StateNothing;
public:
    void ChangeState(State* state);
    State* GetState()const { return m_CurrState; }
};

NS_END