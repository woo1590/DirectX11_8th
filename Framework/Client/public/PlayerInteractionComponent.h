#pragma once
#include "Component.h"

NS_BEGIN(Client)

class Player;
class PlayerInteractionComponent :
    public Component
{
private:
    PlayerInteractionComponent(Object* owner);
    PlayerInteractionComponent(const PlayerInteractionComponent& prototype);
    virtual ~PlayerInteractionComponent() = default;

public:
    static PlayerInteractionComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void SetPlayer(Player* player);
    void EquipWeapon(WeaponID id);
    void HealthUp(_uint health);

    Component* Clone()override { return new PlayerInteractionComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector() {};
#endif 

private:
    Player* m_pPlayer = nullptr;
};

NS_END