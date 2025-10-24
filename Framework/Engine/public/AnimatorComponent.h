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
    void ChangeAnimation(_int animationIndex, _bool isLoop = false, _bool immediateChange = false);

    /*Getter*/
    const std::vector<_float4x4>& GetCombinedMatrices() { return m_CombiendMatirices; }
    const std::vector<_float4x4>& GetTransformationMatrices() { return m_TransformationMatrices; }
    ANIMATIONCLIP_CONTEXT& GetCameraContext() { return m_Context; }
    AnimationClip* GetCurrAnimationClip()const { return m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]; }
    const std::vector<Bone>& GetBones();
    _uint GetNumBones()const;
    _float4x4 GetAdditiveMatrix(_uint boneIndex)const;

    /*Setter*/
    void SetOverride(std::vector<_float4x4> overrideMatrices, std::vector<_uint> masks);
    void SetAdditiveMatrix(_float4x4 matrix, _uint boneIndex);
    void SetFadeDurtaion(_float durtaion) { m_fFadeDuration = durtaion; }

    /*API*/
    BONE_MAP MakeBoneMap(AnimatorComponent* other);
    _bool IsFinished()const { return m_Context.isFinished; }
    void SetPlaySpeedScale(_float scale) { m_Context.playSpeedScale = scale; }
    _float GetProgress();

    Component* Clone() { return new AnimatorComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    void PlayAnimation(_float dt);
    void FadeAnimation(_float dt);
    void ApplyOverride();
    void UpdateCombinedMatrix();

    ANIMATION_SET m_AnimationSet{};
    ANIMATIONCLIP_CONTEXT m_Context{};
    _int m_iCurrAnimationIndex = -1;

    Skeleton* m_pSkeleton = nullptr;
    std::vector<_float4x4> m_TransformationMatrices;
    std::vector<_float4x4> m_CombiendMatirices;

    std::vector<_float4x4> m_AdditiveMatrices;
    std::vector<_float4x4> m_OverrideMatrices;
    std::vector<_uint> m_AdditiveMask;
    std::vector<_uint> m_OverrideMask;

    /*Fade To Next Animation*/
    _bool m_isFade = false;
    _uint m_iNextAnimationIndex{};
    _float m_fFadeTrackPosition{};
    _float m_fFadeDuration = 0.1f;

    std::vector<KEYFRAME> m_CurrKeyFrames;
    std::vector<KEYFRAME> m_NextKeyFrames;
    std::vector<_uint> m_CurrMask;
    std::vector<_uint> m_NextMask;

    std::vector<_uint> m_NextKeyFrameIndices;
    /*-----------------------*/

};

NS_END