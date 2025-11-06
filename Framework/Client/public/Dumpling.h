#pragma once
#include "Item.h"

NS_BEGIN(Client)

class Dumpling :
    public Item
{
private:
    Dumpling();
    Dumpling(const Dumpling& prototype);
    virtual ~Dumpling() = default;

public:
    static Dumpling* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Interaction(PlayerInteractionComponent* interaction)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    class DumplingSpawn : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 1.5f;
    };
    class DumplingIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fMagneticDistance = 30.f;
        _float m_fElapsedTime = 0.f;
        _float3 m_CurrPosition{};
    };
    class DumplingMagnetic : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fMagneticDistance = 30.f;
        _float m_fSpeed = 4.f;
    };

    DumplingSpawn m_DumplingSpawn;
    DumplingIdle m_DumplingIdle;
    DumplingMagnetic m_DumplingMagnetic;
};

NS_END