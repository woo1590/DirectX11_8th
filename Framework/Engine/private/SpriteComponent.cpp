#include "EnginePCH.h"
#include "SpriteComponent.h"
#include "EngineCore.h"
#include "VIBuffer.h"
#include "Material.h"
#include "Object.h"
#include "TransformComponent.h"

SpriteComponent::SpriteComponent(Object* owner)
	:Component(owner)
{
}

SpriteComponent::SpriteComponent(const SpriteComponent& prototype)
	:Component(prototype)
{
}

SpriteComponent* SpriteComponent::Create(Object* owner)
{
	SpriteComponent* Instance = new SpriteComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT SpriteComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT SpriteComponent::Initialize(InitDESC* arg)
{
	if (arg)
	{
		SPRITE_DESC* desc = static_cast<SPRITE_DESC*>(arg);

		m_isAnimated = desc->isAnimated;
		m_isRepeat = desc->isRepeat;
		m_iMaxFrameIndex = desc->iMaxFrameIndex;
		m_fSpeed = desc->fSpeed;
	}

	return S_OK;
}

void SpriteComponent::Update(_float dt)
{
	__super::Update(dt);
}

void SpriteComponent::SetBuffer(_uint levelID, const _string& key)
{
	m_pBuffer = EngineCore::GetInstance()->GetBuffer(levelID, key);
	m_pBuffer->AddRef();
}

void SpriteComponent::SetMaterial(_uint levelID, const _string& key)
{
	m_pMaterial = EngineCore::GetInstance()->GetMaterial(levelID, key);
	m_pMaterial->AddRef();
}

HRESULT SpriteComponent::ExtractRenderProxy(RenderProxy& proxy)
{
	proxy.buffer = m_pBuffer;
	proxy.frameIndex = m_iCurrFrameIndex;
	proxy.material = m_pMaterial;

	return S_OK;
}

void SpriteComponent::Free()
{
	__super::Free();

	Safe_Release(m_pBuffer);
	Safe_Release(m_pMaterial);
}

#ifdef USE_IMGUI

void SpriteComponent::RenderInspector()
{
}

#endif