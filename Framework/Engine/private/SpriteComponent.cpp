#include "EnginePCH.h"
#include "SpriteComponent.h"
#include "EngineCore.h"
#include "VIBuffer.h"
#include "Material.h"
#include "Object.h"
#include "MaterialInstance.h"
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

		m_UseBillBoard = desc->useBillboard;
		m_isAnimated = desc->isAnimated;
		m_isRepeat = desc->isRepeat;
		m_iMaxFrameIndex = desc->iMaxFrameIndex;
		m_fSpeed = desc->fSpeed;
	}

	if (!m_pMaterialInstance)
	{
		m_pMaterialInstance = MaterialInstance::Create();
		if (!m_pMaterialInstance)
			return E_FAIL;
	}

	return S_OK;
}

void SpriteComponent::Update(_float dt)
{
	__super::Update(dt);

	if (!m_isAnimated)
		return;

	m_fElapsedTime += dt * m_fSpeed;

	if (m_fElapsedTime >= 1.f)
	{
		m_iCurrFrameIndex++;
		if (m_iCurrFrameIndex >= m_iMaxFrameIndex)
		{
			if (m_isRepeat)
				m_iCurrFrameIndex = 0;
			else
			{
				m_iCurrFrameIndex--;
				m_isEnd = true;
			}
		}

		m_fElapsedTime = 0.f;
	}
}

void SpriteComponent::SetBuffer(_uint levelID, const _string& key)
{
	m_pBuffer = EngineCore::GetInstance()->GetBuffer(levelID, key);
	m_pBuffer->AddRef();
}

void SpriteComponent::SetMaterial(_uint levelID, const _string& key)
{
	Safe_Release(m_pMaterial);

	m_pMaterial = EngineCore::GetInstance()->GetMaterial(levelID, key);
	m_pMaterial->AddRef();
}

HRESULT SpriteComponent::ExtractRenderProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies)
{
	if (!m_pBuffer || !m_pMaterial)
		return S_OK;

	RenderProxy proxy{};

	proxy.worldMatrix = transform->GetWorldMatrix();
	proxy.buffer = m_pBuffer;
	proxy.frameIndex = m_iCurrFrameIndex;
	proxy.material = m_pMaterial;
	proxy.materialInstance = m_pMaterialInstance;

	proxies.push_back(proxy);

	return S_OK;
}

void SpriteComponent::Free()
{
	__super::Free();

	Safe_Release(m_pMaterialInstance);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pMaterial);
}

#ifdef USE_IMGUI

void SpriteComponent::RenderInspector()
{
}

#endif