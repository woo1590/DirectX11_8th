#pragma once
#include "Item.h"

NS_BEGIN(Client)

class DropWeapon :
    public Item
{
public:
    typedef struct tagDropWeaponDesc : public Object::OBJECT_DESC
    {
        WeaponID weaponID = WeaponID::Count;
    }DROP_WEAPON_DESC;
private:
    DropWeapon();
    DropWeapon(const DropWeapon& prototype);
    virtual ~DropWeapon() = default;

public:
    static DropWeapon* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    void Interaction(PlayerInteractionComponent* interaction)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    WeaponID m_eWeaponID = WeaponID::Count;
    Object* m_pItemGlow = nullptr;

    class DropWeaponSpawn : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float3 m_StartPosition{};
        _float3 m_TargetPosition{};
        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 0.5f;
    };
    class DropWeaponIdel : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float3 m_StartPosition{};
        _float m_fElapsedTime = 0.f;
    };

    DropWeaponSpawn m_DropWeaponSpawn;
    DropWeaponIdel m_DropWeaponIdle;
};

NS_END