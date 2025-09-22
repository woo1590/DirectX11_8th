#include "EnginePCH.h"
#include "AnimatorComponent.h"
#include "EngineCore.h"
#include "AnimationClip.h"
#include "Skeleton.h"

AnimatorComponent::AnimatorComponent(Object* owner)
	:Component(owner)
{
}

AnimatorComponent::AnimatorComponent(const AnimatorComponent& prototype)
	:Component(prototype)
{
}

AnimatorComponent* AnimatorComponent::Create(Object* owner)
{
	AnimatorComponent* Instance = new AnimatorComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT AnimatorComponent::Initialize_Prototype()
{

	return S_OK;
}

HRESULT AnimatorComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

void AnimatorComponent::Update(_float dt)
{
	__super::Update(dt);

	if (!m_pSkeleton || m_TransformationMatirices.empty())
		return;

	if (m_isFade)
		FadeAnimation(dt);
	else
		PlayAnimation(dt);

	UpdateCombinedMatrix();
}

HRESULT AnimatorComponent::SetAnimation(_uint levelID, const _string& key)
{
	m_AnimationSet = EngineCore::GetInstance()->GetAnimation(levelID, key);

	if (!m_AnimationSet.numAnimations)
	{
		MSG_BOX("Empty animation");
		return E_FAIL;
	}

	for (auto& animation : m_AnimationSet.aniamtionClips)
		animation->AddRef();

	return S_OK;
}

void AnimatorComponent::SetSkeleton(Skeleton* pSkeleton)
{
	m_pSkeleton = pSkeleton;
	m_pSkeleton->AddRef();

	const std::vector<Bone>& bones = m_pSkeleton->GetBones();

	for (const auto& bone : bones)
		m_TransformationMatirices.push_back(bone.m_TransformationMatrix);

	_uint numBones = m_TransformationMatirices.size();
	m_CombiendMatirices.resize(numBones);
	m_PrevKeyFrames.resize(numBones);
	m_NextKeyFrames.resize(numBones);
}

void AnimatorComponent::ChangeAnimation(_uint animationIndex, _bool isLoop)
{
	if (-1 == m_iCurrAnimationIndex)	// First Animation
	{
		m_iCurrAnimationIndex = animationIndex;
		m_Context.isLoop = isLoop;
		m_Context.trackPosition = 0.f;
		m_Context.isFinished = false;

		_uint numChannels = m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]->GetNumChannels();
		m_Context.keyFrameIndices.assign(numChannels, 0);
	}
	else
	{
		/*Animation Fade Setting*/

		m_iPrevAnimationIndex = m_iCurrAnimationIndex;
		m_iCurrAnimationIndex = animationIndex;
	
		m_Context.isLoop = isLoop;
		m_Context.trackPosition = 0.f;
		m_Context.isFinished = false;

		if (m_isFade)	//Already Animation Fade
		{
			_uint numChannels = m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]->GetNumChannels();
			m_Context.keyFrameIndices.assign(numChannels, 0);
		}
		else
		{

		}
		m_isFade = true;
		m_fFadeTrackPosition = 0.f;

		/*Init Next KeyFrame Indices*/
		_uint numChannels = m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]->GetNumChannels();
		m_NextKeyFrameIndices.assign(numChannels, 0);

		/*Init KeyFrame Buffers & Mask*/
		_uint numBones = m_pSkeleton->GetBones().size();
		m_PrevKeyFrames.assign(numBones, KEYFRAME{});
		m_NextKeyFrames.assign(numBones, KEYFRAME{});

		m_PrevMask.assign(numBones, 0);
		m_NextMask.assign(numBones, 0);

		/*Extract KeyFrames*/
		m_AnimationSet.aniamtionClips[m_iPrevAnimationIndex]->ExtractKeyFrames(m_PrevKeyFrames, m_PrevMask, m_Context.keyFrameIndices);
		m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]->ExtractKeyFrames(m_NextKeyFrames, m_NextMask, m_NextKeyFrameIndices);
	}
}

void AnimatorComponent::Free()
{
	__super::Free();

	for (auto& animation : m_AnimationSet.aniamtionClips)
		Safe_Release(animation);

	Safe_Release(m_pSkeleton);
}

#ifdef USE_IMGUI

void AnimatorComponent::RenderInspector()
{

}

#endif

void AnimatorComponent::PlayAnimation(_float dt)
{
	if (m_iCurrAnimationIndex >= m_AnimationSet.numAnimations)
		return;

	m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]->UpdateTransformationMatrix(dt, m_Context, m_TransformationMatirices);
}

void AnimatorComponent::FadeAnimation(_float dt)
{
	m_fFadeTrackPosition += dt;

	if (m_fFadeTrackPosition >= m_fFadeDuration)
	{
		m_isFade = false;	//Fade End!

		_uint numChannels = m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]->GetNumChannels();
		m_Context.keyFrameIndices.assign(numChannels, 0);
		return;
	}

	for (_uint i = 0; i < m_PrevKeyFrames.size(); ++i)
	{
		KEYFRAME prevKeyFrame = m_PrevKeyFrames[i];
		KEYFRAME nextKeyFrame = m_NextKeyFrames[i];

		if (m_PrevMask[i] && m_NextMask[i])	//키프레임이 둘 다 존재 -> 보간
		{
			_vector scale, rotation, position;
			
			_vector scaleA = XMLoadFloat3(&prevKeyFrame.scale);
			_vector rotationA = XMLoadFloat4(&prevKeyFrame.rotation);
			_vector positionA = XMVectorSetW(XMLoadFloat3(&prevKeyFrame.position), 1.f);
			
			_vector scaleB = XMLoadFloat3(&nextKeyFrame.scale);
			_vector rotationB = XMLoadFloat4(&nextKeyFrame.rotation);
			_vector positionB = XMVectorSetW(XMLoadFloat3(&nextKeyFrame.position), 1.f);

			_float t = m_fFadeTrackPosition / m_fFadeDuration;
			t = std::clamp(t, 0.f, 1.f);

			scale = XMVectorLerp(scaleA, scaleB, t);
			rotation = XMQuaternionSlerp(rotationA, rotationB, t);
			position = XMVectorSetW(XMVectorLerp(positionA, positionB, t), 1.f);

			_vector rotationOrigin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
			_matrix transformation = XMMatrixAffineTransformation(scale, rotationOrigin, rotation, position);

			XMStoreFloat4x4(&m_TransformationMatirices[i], transformation);
		}
	}
}

void AnimatorComponent::UpdateCombinedMatrix()
{
	auto& bones = m_pSkeleton->GetBones();
	_float4x4 preTransformMatrix = m_pSkeleton->GetPreTransformMatrix();
	
	for (_uint i = 0; i < bones.size(); ++i)
	{
		_int parentBoneIndex = bones[i].m_iParentIndex;

		if (-1 == parentBoneIndex)
			XMStoreFloat4x4(&m_CombiendMatirices[i], XMLoadFloat4x4(&m_TransformationMatirices[i]) * XMLoadFloat4x4(&preTransformMatrix));
		else
			XMStoreFloat4x4(&m_CombiendMatirices[i], XMLoadFloat4x4(&m_TransformationMatirices[i]) * XMLoadFloat4x4(&m_CombiendMatirices[parentBoneIndex]));
	}
}
