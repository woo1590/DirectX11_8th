#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class Skeleton;
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

    HRESULT     SetAnimation(_uint levelID, const _string& key);
    void        SetSkeleton(Skeleton* pSkeleton);
    void        ChangeAnimation(_uint animationIndex, _bool isLoop = false);

    const std::vector<_float4x4>& GetCombinedMatrices() { return m_CombiendMatirices; }
    Component* Clone() { return new AnimatorComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    void PlayAnimation(_float dt);
    void FadeAnimation(_float dt);
    void UpdateCombinedMatrix();

    ANIMATION_SET m_AnimationSet{};
    ANIMATIONCLIP_CONTEXT m_Context{};
    _uint m_iCurrAnimationIndex = -1;

    Skeleton* m_pSkeleton = nullptr;
    std::vector<_float4x4> m_TransformationMatirices;
    std::vector<_float4x4> m_CombiendMatirices;

    /*Fade To Next Animation*/
    _bool m_isFade = false;
    _uint m_iNextAnimationIndex{};
    _float m_fFadeTrackPosition{};
    _float m_fFadeDuration = 0.4f;

    std::vector<KEYFRAME> m_CurrKeyFrames;
    std::vector<KEYFRAME> m_NextKeyFrames;
    std::vector<_uint> m_CurrMask;
    std::vector<_uint> m_NextMask;

    std::vector<_uint> m_NextKeyFrameIndices;
    /*-----------------------*/

};

NS_END