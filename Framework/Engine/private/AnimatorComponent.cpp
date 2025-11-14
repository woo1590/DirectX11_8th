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

	if (!m_pSkeleton || m_TransformationMatrices.empty())
		return;

	if (m_isFade)
	{
		FadeAnimation(dt);
	}
	else
	{
		PlayAnimation(dt);
		ApplyOverride();
	}

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
		m_TransformationMatrices.push_back(bone.m_TransformationMatrix);

	_uint numBones = m_TransformationMatrices.size();
	m_CombiendMatirices.resize(numBones);
	m_CurrKeyFrames.resize(numBones);
	m_NextKeyFrames.resize(numBones);
	
	/*Init override layer*/
	m_OverrideMatrices.resize(numBones);
	m_OverrideMask.resize(numBones);

	/*Init additive layer*/
	m_AdditiveMatrices.resize(numBones);
	m_AdditiveMask.resize(numBones);
	
	for (_uint i = 0; i < numBones; ++i)
		XMStoreFloat4x4(&m_AdditiveMatrices[i], XMMatrixIdentity());
}

void AnimatorComponent::ChangeAnimation(_int animationIndex, _bool isLoop, _bool immediateChange)
{
	if (-1 == m_iCurrAnimationIndex || m_Context.isFinished || immediateChange)	// First Animation and PreAnimation finished
	{
		m_iCurrAnimationIndex = animationIndex;
		m_Context.isLoop = isLoop;
		m_Context.trackPosition = 0.f;
		m_Context.isFinished = false;
		m_Context.playSpeedScale = 1.f;

		_uint numChannels = m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]->GetNumChannels();
		m_Context.keyFrameIndices.assign(numChannels, 0);
	}
	else
	{
		/*Animation Fade Setting*/
		m_iNextAnimationIndex = animationIndex;

		m_isFade = true;
		m_fFadeTrackPosition = 0.f;
	
		m_Context.isLoop = isLoop;
		m_Context.trackPosition = 0.f;
		m_Context.isFinished = false;
		m_Context.playSpeedScale = 1.f;

		/*Init KeyFrame Buffers & Mask*/
		_uint numBones = m_pSkeleton->GetBones().size();
		m_CurrKeyFrames.assign(numBones, KEYFRAME{});
		m_NextKeyFrames.assign(numBones, KEYFRAME{});

		m_CurrMask.assign(numBones, 0);
		m_NextMask.assign(numBones, 0);

		/*Init Next KeyFrame Indices*/
		_uint numChannels = m_AnimationSet.aniamtionClips[m_iNextAnimationIndex]->GetNumChannels();
		m_NextKeyFrameIndices.assign(numChannels, 0);

		/*Extract KeyFrames*/
		m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]->ExtractKeyFrames(m_CurrKeyFrames, m_CurrMask, m_Context.keyFrameIndices);
		m_AnimationSet.aniamtionClips[m_iNextAnimationIndex]->ExtractKeyFrames(m_NextKeyFrames, m_NextMask, m_NextKeyFrameIndices);
	}
}

_float4x4* AnimatorComponent::GetCombinedMatrixByIndex_PTR(_uint index)
{
	if (index >= m_CombiendMatirices.size())
		return nullptr;

	return &m_CombiendMatirices[index];
}

_uint AnimatorComponent::GetNumBones() const
{
	return m_pSkeleton->GetBones().size();
}

_float4x4 AnimatorComponent::GetAdditiveMatrix(_uint boneIndex) const
{
	_float4x4 additive{};
	XMStoreFloat4x4(&additive, XMMatrixIdentity());

	if (boneIndex >= m_AdditiveMatrices.size())
		return additive;

	return m_AdditiveMatrices[boneIndex];
}

const std::vector<Bone>& AnimatorComponent::GetBones()
{
	return m_pSkeleton->GetBones();
}

void AnimatorComponent::SetOverride(std::vector<_float4x4> overrideMatrices, std::vector<_uint> masks)
{
	m_OverrideMatrices = overrideMatrices;
	m_OverrideMask = masks;
}

void AnimatorComponent::SetAdditiveMatrix(_float4x4 matrix, _uint boneIndex)
{
	m_AdditiveMatrices[boneIndex] = matrix;
	m_AdditiveMask[boneIndex] = 1;
}

BONE_MAP AnimatorComponent::MakeBoneMap(AnimatorComponent* other)
{
	BONE_MAP boneMap{};
	const std::vector<Bone>& otherBones = other->GetBones();
	for (const auto& otherBone : otherBones)
	{
		_int index = m_pSkeleton->GetBoneIndexByName(otherBone.m_strBoneTag);
		boneMap.mappedBoneIndex.push_back(index);
		boneMap.nameToIndex.emplace(otherBone.m_strBoneTag, index);
	}

	return boneMap;
}

_float AnimatorComponent::GetProgress()
{
	_float animDuration = m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]->GetDuration();
	_float progress = std::clamp(m_Context.trackPosition / animDuration, 0.f, 1.f);

	return progress;
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
	ImGui::PushID(this);

	_string animationName = m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]->GetAnimationName();
	ImGui::Text(animationName.c_str());

	ImGui::PopID();
}

#endif

void AnimatorComponent::PlayAnimation(_float dt)
{
	if (m_iCurrAnimationIndex >= m_AnimationSet.numAnimations)
		return;

	m_AnimationSet.aniamtionClips[m_iCurrAnimationIndex]->UpdateTransformationMatrix(dt, m_Context, m_TransformationMatrices);
}

void AnimatorComponent::FadeAnimation(_float dt)
{
	m_fFadeTrackPosition += dt;

	if (m_fFadeTrackPosition >= m_fFadeDuration)
	{
		m_isFade = false;	/*Fade End!*/
		m_fFadeDuration = 0.1f; /*Default fae duration*/

		m_iCurrAnimationIndex = m_iNextAnimationIndex;
		m_Context.trackPosition = 0.f;

		_uint numChannels = m_NextKeyFrameIndices.size();
		m_Context.keyFrameIndices.assign(numChannels, 0);

		return;
	}

	for (_uint i = 0; i < m_CurrKeyFrames.size(); ++i)
	{
		KEYFRAME currKeyFrame = m_CurrKeyFrames[i];
		KEYFRAME nextKeyFrame = m_NextKeyFrames[i];

		if (m_CurrMask[i] && m_NextMask[i])	//키프레임이 둘 다 존재 -> 보간
		{
			_vector scale, rotation, position;
			
			_vector scaleA = XMLoadFloat3(&currKeyFrame.scale);
			_vector rotationA = XMLoadFloat4(&currKeyFrame.rotation);
			_vector positionA = XMVectorSetW(XMLoadFloat3(&currKeyFrame.position), 1.f);
			
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

			XMStoreFloat4x4(&m_TransformationMatrices[i], transformation);
		}
	}
}

void AnimatorComponent::ApplyOverride()
{
	auto& bones = m_pSkeleton->GetBones();
	for (_uint i = 0; i < bones.size(); ++i)
	{
		if (!m_OverrideMask[i])
			continue;

		m_TransformationMatrices[i] = m_OverrideMatrices[i];
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
			XMStoreFloat4x4(&m_CombiendMatirices[i], XMLoadFloat4x4(&m_TransformationMatrices[i])
												   * XMLoadFloat4x4(&m_AdditiveMatrices[i])
												   * XMLoadFloat4x4(&preTransformMatrix));
		else
			XMStoreFloat4x4(&m_CombiendMatirices[i], XMLoadFloat4x4(&m_TransformationMatrices[i]) 
												   * XMLoadFloat4x4(&m_AdditiveMatrices[i]) 
												   * XMLoadFloat4x4(&m_CombiendMatirices[parentBoneIndex]));
	}
}
