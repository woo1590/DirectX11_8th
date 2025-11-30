#include "pch.h"	
#include "MinimapArea.h"
#include "MaterialInstance.h"
#include "VIBuffer.h"
#include "Material.h"
#include "Shader.h"

//component
#include "SpriteComponent.h"

MinimapArea::MinimapArea()
	:UIObject()
{
}

MinimapArea::MinimapArea(const MinimapArea& prototype)
	:UIObject(prototype)
{
}

MinimapArea* MinimapArea::Create()
{
	MinimapArea* Instance = new MinimapArea();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MinimapArea::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT MinimapArea::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.isAnimated = false;
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_MinimapArea");
	sprite->GetMaterialInstance()->SetTexture("g_MinimapTexture", engine->GetSRV("Target_Minimap"));

	auto mtrlInstance = sprite->GetMaterialInstance();
	mtrlInstance->SetPass("MinimapArea_Pass");

	m_pBuffer = engine->GetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pPlayerIcon = engine->GetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_MinimapPlayerIcon");
	m_pEnemyIcon = engine->GetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_MinimapEnemyIcon");
	m_pDoorIcon = engine->GetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_DoorMark");
	m_pMaterialInstance = MaterialInstance::Create();
	m_pMaterialInstance->SetPass("Minimap_Pass");

	m_pBuffer->AddRef();
	m_pPlayerIcon->AddRef();
	m_pEnemyIcon->AddRef();
	m_pDoorIcon->AddRef();

	engine->Subscribe(ENUM_CLASS(EventID::DoorActive), MakeListener(&MinimapArea::DoorMarkActive));
	engine->Subscribe(ENUM_CLASS(EventID::DoorDeactive), MakeListener(&MinimapArea::DoorMarkDeactive));

	return S_OK;
}

void MinimapArea::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void MinimapArea::Update(_float dt)
{
	__super::Update(dt);
}

void MinimapArea::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT MinimapArea::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	//__super::ExtractRenderProxies(proxies);
	RenderProxy proxy{};
	proxy.renderFunc = [this]()->HRESULT {return DrawMinimap(); };
	proxies[ENUM_CLASS(RenderGroup::CustomPass)].push_back(proxy);

	__super::ExtractRenderProxies(proxies);

	return S_OK;
}

HRESULT MinimapArea::DrawMinimap()
{
	auto engine = EngineCore::GetInstance();
	engine->BeginMRT("MRT_Minimap");

	/*player icon*/
	{
		m_pMaterialInstance->SetFloat2("g_MinimapPosition", _float2{ 0.f,0.f });
		m_pMaterialInstance->SetFloat2("g_MinimapIconSize", _float2{ 0.3f,0.3f });
		
		m_pBuffer->BindBuffers();
		m_pPlayerIcon->BindMaterial(0, m_pMaterialInstance);
		m_pBuffer->Draw();
	}

	/*enemy icon*/
	{
		m_pMaterialInstance->SetFloat2("g_MinimapIconSize", _float2{ 0.15f,0.15f });

		auto& enemies = engine->GetObjects(engine->GetCurrLevelID(), "Layer_Enemy");
		for (const auto& enemy : enemies)
		{
			_float2 minimapPosition = ComputeMinimapPosition(enemy->GetComponent<TransformComponent>()->GetPosition());

			m_pMaterialInstance->SetFloat2("g_MinimapPosition", minimapPosition);
			m_pBuffer->BindBuffers();
			m_pEnemyIcon->BindMaterial(0, m_pMaterialInstance);
			m_pBuffer->Draw();
		}
	}

	/*door icon*/
	if (m_IsDoorActive)
	{
		m_pMaterialInstance->SetFloat2("g_MinimapIconSize", _float2{ 0.3f,0.3f });
		_float2 minimapPosition = ComputeMinimapPosition(m_DoorPosition);

		m_pMaterialInstance->SetFloat2("g_MinimapPosition", minimapPosition);
		m_pBuffer->BindBuffers();
		m_pDoorIcon->BindMaterial(0, m_pMaterialInstance);
		m_pBuffer->Draw();
	}

	engine->EndMRT();

	return S_OK;
}

void MinimapArea::DoorMarkActive(std::any param)
{
	m_IsDoorActive = true;
	m_DoorPosition = std::any_cast<_float3>(param);
}

void MinimapArea::DoorMarkDeactive(std::any param)
{
	m_IsDoorActive = false;
}

Object* MinimapArea::Clone(InitDESC* arg)
{
	MinimapArea* Instance = new MinimapArea(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void MinimapArea::Free()
{
	__super::Free();

	Safe_Release(m_pBuffer);
	Safe_Release(m_pPlayerIcon);
	Safe_Release(m_pEnemyIcon);
	Safe_Release(m_pDoorIcon);
	Safe_Release(m_pMaterialInstance);
}

_float2 MinimapArea::ComputeMinimapPosition(_float3 worldPosition)
{
	auto engine = EngineCore::GetInstance();

	auto playerTransform = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player")->GetComponent<TransformComponent>();
	_float3 playerPos = playerTransform->GetPosition();
	_float3 playerForward = playerTransform->GetForward();
	_float3 playerRight = playerTransform->GetRight();

	_float3 dir{};
	XMStoreFloat3(&dir, XMLoadFloat3(&worldPosition) - XMLoadFloat3(&playerPos));

	_float2 localForward{ playerForward.x,playerForward.z };
	_float2 localRight{ playerRight.x,playerRight.z };
	_float2 localDir{ dir.x,dir.z };
	_float localDistance = XMVectorGetX(XMVector2Length(XMLoadFloat2(&localDir))) / m_fMinimapDistance;

	XMStoreFloat2(&localForward, XMVector2Normalize(XMLoadFloat2(&localForward)));
	XMStoreFloat2(&localRight, XMVector2Normalize(XMLoadFloat2(&localRight)));
	XMStoreFloat2(&localDir, XMVector2Normalize(XMLoadFloat2(&localDir)));

	_float x = XMVectorGetX(XMVector2Dot(XMLoadFloat2(&localRight), XMLoadFloat2(&localDir))) * localDistance;
	_float y = XMVectorGetX(XMVector2Dot(XMLoadFloat2(&localForward), XMLoadFloat2(&localDir))) * localDistance;

	return _float2{ x,y };
}
