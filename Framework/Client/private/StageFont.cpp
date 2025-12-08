#include "pch.h"
#include "StageFont.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

StageFont::StageFont()
	:Object()
{
}

StageFont::StageFont(const StageFont& prototype)
	:Object(prototype)
{
}

StageFont* StageFont::Create()
{
	StageFont* Instance = new StageFont();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT StageFont::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT StageFont::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	STAGE_FONT_DESC* desc = static_cast<STAGE_FONT_DESC*>(arg);

	switch (desc->stageID)
	{
	case LevelID::Stage1:
		m_strStageText = L"½ºÅ×ÀÌÁö 1";
		break;
	case LevelID::Stage2:
	{
		m_strStageText = L"½ºÅ×ÀÌÁö 2";
		m_pTransform->Rotate(_float3{ 0.f,math::ToRadian(180.f),0.f });
	}break;
	case LevelID::Stage3:
	{
		m_strStageText = L"½ºÅ×ÀÌÁö 3";
		m_pTransform->Rotate(_float3{ 0.f,math::ToRadian(90.f),0.f });
	}break;
	default:
		break;
	}

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.isAnimated = false;
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Button");

	auto mtrlInstance = sprite->GetMaterialInstance();
	mtrlInstance->SetPass("StageFont_Pass");
	mtrlInstance->SetTexture("g_UIButtonTexture", engine->GetSRV("Target_UIButton"));

	_float2 fontSize{ 123.f * 3.f,123.f * 3.f };
	mtrlInstance->SetFloat2("g_ButtonUVMin", _float2{ 0.5f - (fontSize.x * 0.5f / WinSizeX) ,0.5f - (fontSize.y * 0.5f / WinSizeY) });
	mtrlInstance->SetFloat2("g_ButtonUVMax", _float2{ 0.5f + (fontSize.x * 0.5f / WinSizeX) ,0.5f + (fontSize.y * 0.5f / WinSizeY) });
	mtrlInstance->SetFloat("g_CustomAlpha", 1.f);

	/*LT*/
	m_Offsets.push_back(_float2{ -1.f,-1.f });
	/*TC*/
	m_Offsets.push_back(_float2{ 0.f,-1.f });
	/*RT*/
	m_Offsets.push_back(_float2{ 1.f,-1.f });
	/*RC*/
	m_Offsets.push_back(_float2{ -1.f,0.f });
	/*RB*/
	m_Offsets.push_back(_float2{ 1.f,1.f });
	/*BC*/
	m_Offsets.push_back(_float2{ 0.f,1.f });
	/*LB*/
	m_Offsets.push_back(_float2{ -1.f,1.f });
	/*LC*/
	m_Offsets.push_back(_float2{ -1.f,0.f });

	m_pTransform->SetScale(_float3{ 35.f,35.f,1.f });
	ChangeState(&m_StageFontIdle);

	return S_OK;
}

void StageFont::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void StageFont::Update(_float dt)
{
	__super::Update(dt);
}

void StageFont::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT StageFont::DrawFont()
{
	auto engine = EngineCore::GetInstance();

	engine->BeginMRT("MRT_UIButton");

	for (_uint i = 0; i < 8; ++i)
	{
		_float2 offset = m_Offsets[i];

		FONT_PROXY font{};
		font.fontTag = "Nexon_Font";
		font.color = _float4(0.f, 0.f, 0.f, 1.f);
		font.scale = 1.f;
		font.screenPosition = _float3{ WinSizeX * 0.5f + offset.x * 1.5f - 150.f,WinSizeY * 0.5f + offset.y * 1.5f - 30.f,0.f };
		font.text = L"¿ë¸ª ¹«´ý";
		EngineCore::GetInstance()->DrawFont(font);
	}

	for (_uint i = 0; i < 8; ++i)
	{
		_float2 offset = m_Offsets[i];

		FONT_PROXY font{};
		font.fontTag = "Nexon_Font";
		font.color = _float4(0.f, 0.f, 0.f, 1.f);
		font.scale = 0.5f;
		font.screenPosition = _float3{ WinSizeX * 0.5f + offset.x * 2.f - 100.f,WinSizeY * 0.56f + offset.y * 2.f,0.f };
		font.text = m_strStageText;
		EngineCore::GetInstance()->DrawFont(font);
	}

	{
		FONT_PROXY font{};
		font.fontTag = "Nexon_Font";
		font.color = _float4{ 223.f / 255.f,223.f / 255.f,237.f / 255.f,1.f };
		font.text = L"¿ë¸ª ¹«´ý";
		font.screenPosition = _float3{ WinSizeX * 0.5f - 150.f,WinSizeY * 0.5f - 30.f,0.f };
		font.scale = 1.f;
		EngineCore::GetInstance()->DrawFont(font);
	}
	
	{
		FONT_PROXY font{};
		font.fontTag = "Nexon_Font";
		font.color = _float4{ 1.f,1.f,1.f,1.f };
		font.text = m_strStageText;
		font.screenPosition = _float3{ WinSizeX * 0.5f - 100.f,WinSizeY * 0.56f,0.f };
		font.scale = 0.5f;
		EngineCore::GetInstance()->DrawFont(font);
	}

	engine->EndMRT();


	return S_OK;
}

HRESULT StageFont::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	RenderProxy proxy{};
	proxy.renderFunc = [this]()->HRESULT {return DrawFont(); };
	proxies[ENUM_CLASS(RenderGroup::CustomPass)].push_back(proxy);

	__super::ExtractRenderProxies(proxies);

	return S_OK;
}

Object* StageFont::Clone(InitDESC* arg)
{
	StageFont* Instance = new StageFont(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void StageFont::Free()
{
	__super::Free();
}

void StageFont::StageFontIdle::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
}

void StageFont::StageFontIdle::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;
}

void StageFont::StageFontIdle::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto stageFont = static_cast<StageFont*>(object);
		stageFont->ChangeState(&stageFont->m_StageFontDisappear);
	}
}

void StageFont::StageFontDisappear::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
}

void StageFont::StageFontDisappear::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	if (m_fElapsedTime < m_fDuration)
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = math::EaseOutSline(t);

		auto mtrlInstance = object->GetComponent<SpriteComponent>()->GetMaterialInstance();
		mtrlInstance->SetFloat("g_CustomAlpha", 1.f - t);
	}
}

void StageFont::StageFontDisappear::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		object->SetDead();
	}
}
