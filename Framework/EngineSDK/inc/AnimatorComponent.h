#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class Bone;
class ENGINE_DLL AnimatorComponent final:
    public Component
{
private:
    AnimatorComponent(Object* owner);
    AnimatorComponent(const AnimatorComponent& prototype);
    virtual ~AnimatorComponent() = default;

public:
    static AnimatorComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;

    HRESULT SetAnimation(_uint levelID, const _string& key);
    void SetModelBones(std::vector<Bone*>* modelBones) { m_ModelBones = modelBones; }

    Component* Clone() { return new AnimatorComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:
    ANIMATION_SET m_AnimationSet{};
    _uint m_iCurrAnimationIndex{};

    const std::vector<Bone*>* m_ModelBones;

    std::vector<_float4x4> m_TransformationMatrix;
    std::vector<_float4x4> m_CombiendTransformationMatrix;
    std::vector<_float4x4> m_FinalTransformationMatrix;
};

NS_END