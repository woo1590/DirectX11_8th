#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class AnimatorComponent;
class ModelComponent;
class Skeleton;
NS_END

NS_BEGIN(Client)

class PlayerAnimController :
    public Component
{
private:
    PlayerAnimController(Object* owner);
    PlayerAnimController(const PlayerAnimController& prototype);
    virtual ~PlayerAnimController() = default;

public:
    static PlayerAnimController* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;

    void SetWeaponAnimator(AnimatorComponent* weaponAnimator);
    void SetHandAnimator(AnimatorComponent* handAnimator);
    void SetBoneMap(BONE_MAP boneMap);
    Component* Clone()override { return new PlayerAnimController(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:
    AnimatorComponent* m_pHandAnimator = nullptr;
    AnimatorComponent* m_pWeaponAnimator = nullptr;
    BONE_MAP m_BoneMap{};

    _uint m_iNumHandBones{};
    Skeleton* m_pHandSkelton = nullptr;
    std::vector<_float4x4> m_HandOverrideMatrices;
    std::vector<_uint> m_HandMasks;
};

NS_END