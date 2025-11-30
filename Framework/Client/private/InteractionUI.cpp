#include "pch.h"
#include "InteractionUI.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

InteractionUI::InteractionUI()
	:UIObject()
{
}

InteractionUI::InteractionUI(const InteractionUI& prototype)
	:UIObject(prototype)
{
}

InteractionUI* InteractionUI::Create()
{
	InteractionUI* Instance = new InteractionUI();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT InteractionUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT InteractionUI::Initialize(InitDESC* arg)
{
	UIObject::UIOBJECT_DESC desc{};
	desc.sizeX = 30.f;
	desc.sizeY = 30.f;
	desc.x = WinSizeX * 0.5f + WinSizeX * 0.1f;
	desc.y = WinSizeY * 0.5f + 80.f;

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_InteractionFrame");
	sprite->GetMaterialInstance()->SetPass("CustomAlpha_Pass");

	m_pInteractionKey = SpriteComponent::Create(this);
	m_pInteractionKey->Initialize(&spriteDesc);
	m_pInteractionKey->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pInteractionKey->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_InteractionKey");
	m_pInteractionKey->GetMaterialInstance()->SetPass("CustomAlpha_Pass");

	m_pKeyTransform = TransformComponent::Create(this);
	m_pKeyTransform->SetPosition(m_pTransform->GetPosition());
	m_pKeyTransform->SetScale(_float3(desc.sizeX * 0.8f, desc.sizeY * 0.8f, 1.f));

	engine->Subscribe(ENUM_CLASS(EventID::InteractionWeapon), MakeListener(&InteractionUI::InteractionWeapon));
	engine->Subscribe(ENUM_CLASS(EventID::InteractionDoor), MakeListener(&InteractionUI::InteractionDoor));
	engine->Subscribe(ENUM_CLASS(EventID::InteractionChest), MakeListener(&InteractionUI::InteractionChest));
	engine->Subscribe(ENUM_CLASS(EventID::InteractionDeactive), MakeListener(&InteractionUI::InteractionDeactive));

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

	return S_OK;
}

void InteractionUI::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void InteractionUI::Update(_float dt)
{
	__super::Update(dt);

	if (m_OnActive)
	{
		auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();

		m_fElpasedTime += dt;
		if (m_fElpasedTime >= m_fDuration)
		{
			m_IsActive = true;
			m_OnActive = false;
			m_fElpasedTime = 0.f;
			mtrlInstance->SetFloat("g_CustomAlpha", 1.f);
			return;
		}

		_float t = m_fElpasedTime / m_fDuration;
		mtrlInstance->SetFloat("g_CustomAlpha", t);
		m_fFontAlpha = t;
	}

	if (m_OnDeactive)
	{
		auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();

		m_fElpasedTime += dt;
		if (m_fElpasedTime >= m_fDuration)
		{
			m_IsActive = false;
			m_OnDeactive = false;
			m_fElpasedTime = 0.f;
			mtrlInstance->SetFloat("g_CustomAlpha", 0.f);
			return;
		}

		_float t = m_fElpasedTime / m_fDuration;
		mtrlInstance->SetFloat("g_CustomAlpha", 1.f - t);
		m_fFontAlpha = 1.f - t;
	}
}

void InteractionUI::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT InteractionUI::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	if (!m_IsActive)
		return S_OK;

	__super::ExtractRenderProxies(proxies);

	if (m_pInteractionKey)
		m_pInteractionKey->ExtractRenderProxy(m_pKeyTransform, proxies[ENUM_CLASS(RenderGroup::UI)]);

	_float3 position = m_pTransform->GetPosition();

	{
		for (_uint i = 0; i < 8; ++i)
		{
			_float2 offset = m_Offsets[i];

			FONT_PROXY font{};
			font.fontTag = "Default_Font";
			font.color = _float4(0.f, 0.f, 0.f, m_fFontAlpha);
			font.scale = 0.4f;
			font.screenPosition = _float3{ m_fX + offset.x * 1.3f + 20.f ,m_fY + offset.y * 1.3f - 12.f,0.f };
			font.text = m_strInteractionText;
			EngineCore::GetInstance()->AddFontProxy(font);
		}
	}

	{
		FONT_PROXY font{};
		font.fontTag = "Default_Font";
		font.color = _float4(1.f, 1.f, 1.f, m_fFontAlpha);
		font.scale = 0.4f;
		font.screenPosition = _float3{ m_fX + 20.f,m_fY - 12.f,0.f };
		font.text = m_strInteractionText;
		EngineCore::GetInstance()->AddFontProxy(font);
	}


	return S_OK;
}

void InteractionUI::InteractionWeapon(std::any param)
{
	if (m_IsActive)
		return;

	m_fElpasedTime = 0.f;
	m_IsActive = true;
	m_OnActive = true;
	m_strInteractionText = L"줍기";
}

void InteractionUI::InteractionDoor(std::any param)
{
	if (m_IsActive)
		return;

	m_fElpasedTime = 0.f;
	m_IsActive = true;
	m_OnActive = true;
	m_strInteractionText = L"다음 스테이지로 진행하기";
}

void InteractionUI::InteractionChest(std::any param)
{
	if (m_IsActive)
		return;

	m_fElpasedTime = 0.f;
	m_IsActive = true;
	m_OnActive = true;
	m_strInteractionText = L"상자 열기";
}

void InteractionUI::InteractionDeactive(std::any param)
{
	m_fElpasedTime = 0.f;
	m_OnDeactive = true;
	//m_strInteractionText = L"";
}

Object* InteractionUI::Clone(InitDESC* arg)
{
	InteractionUI* Instance = new InteractionUI(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void InteractionUI::Free()
{
	__super::Free();
	Safe_Release(m_pInteractionKey);
	Safe_Release(m_pKeyTransform);
}
