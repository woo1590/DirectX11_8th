#pragma once
#include "Projectile.h"

NS_BEGIN(Client)

class Dynamite :
    public Projectile
{
public:
    typedef struct tagDynamtieDesc : public Object::OBJECT_DESC
    {
        _float3 velocity{};
    }DYNAMITE_DESC;
private:
    Dynamite();
    Dynamite(const Dynamite& prototype);
    virtual ~Dynamite() = default;

public:
    static Dynamite* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float3 m_Velocity{};

    class DynamiteIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class DynamiteDead : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 0.2f;
    };

    DynamiteIdle m_DynamiteIdle;
    DynamiteDead m_DynamiteDead;
};

NS_END