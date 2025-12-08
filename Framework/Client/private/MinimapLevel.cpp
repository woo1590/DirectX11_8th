#include "pch.h"
#include "MinimapLevel.h"

//component
#include "SpriteComponent.h"

MinimapLevel::MinimapLevel()
	:UIObject()
{
}

MinimapLevel::MinimapLevel(const MinimapLevel& prototype)
	:UIObject(prototype)
{
}

MinimapLevel* MinimapLevel::Create()
{
	MinimapLevel* Instance = new MinimapLevel();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MinimapLevel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT MinimapLevel::Initialize(InitDESC* arg)
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
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_MinimapLevel");

	return S_OK;
}

HRESULT MinimapLevel::LateInitialize()
{
	__super::LateInitialize();

	_uint levelID = EngineCore::GetInstance()->GetCurrLevelID();

	switch (static_cast<LevelID>(levelID))
	{
	case LevelID::Stage1:
		m_strStageTag = L"¿ë¸ª ¹«´ý - ½ºÅ×ÀÌÁö1";
		break;
	case LevelID::Stage2:
		m_strStageTag = L"¿ë¸ª ¹«´ý - ½ºÅ×ÀÌÁö2";
		break;
	case LevelID::Stage3:
		m_strStageTag = L"¿ë¸ª ¹«´ý - ½ºÅ×ÀÌÁö3";
		break;
	case LevelID::StageBoss:
		m_strStageTag = L"¿ë¸ª ¹«´ý - Àå»ýÀü";
		break;
	default:
		break;
	}

	return S_OK;
}

void MinimapLevel::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void MinimapLevel::Update(_float dt)
{
	__super::Update(dt);
}

void MinimapLevel::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT MinimapLevel::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	__super::ExtractRenderProxies(proxies);

	_float3 position = m_pTransform->GetWorldPosition();
	_float3 scale = m_pTransform->GetScale();

	FONT_PROXY font{};
	font.fontTag = "Default_Font";
	font.color = _float4{ 189.f / 255.f,172.f / 255.f,128.f / 255.f,1.f };
	font.scale = 0.33f;
	font.screenPosition.x = position.x + WinSizeX * 0.48f - scale.x * 0.26f;
	font.screenPosition.y = WinSizeY * 0.5f - position.y - scale.y * 0.42f;
	font.text = m_strStageTag;

	EngineCore::GetInstance()->AddFontProxy(font);

	return S_OK;
}

Object* MinimapLevel::Clone(InitDESC* arg)
{
	MinimapLevel* Instance = new MinimapLevel(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void MinimapLevel::Free()
{
	__super::Free();
}