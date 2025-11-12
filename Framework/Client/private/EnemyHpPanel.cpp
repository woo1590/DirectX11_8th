#include "pch.h"
#include "EnemyHpPanel.h"
#include "MaterialInstance.h"
#include "Bar.h"

//component
#include "SpriteComponent.h"

EnemyHpPanel::EnemyHpPanel()
	:UIObject()
{
}

EnemyHpPanel::EnemyHpPanel(const EnemyHpPanel& prototype)
	:UIObject(prototype)
{
}

EnemyHpPanel* EnemyHpPanel::Create()
{
	EnemyHpPanel* Instance = new EnemyHpPanel();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EnemyHpPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT EnemyHpPanel::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	ENEMY_HP_PANEL_DESC* desc = static_cast<ENEMY_HP_PANEL_DESC*>(arg);

	auto engine = EngineCore::GetInstance();
	engine->Subscribe(ENUM_CLASS(EventID::EnemyHpPanelPositionUpdate), MakeListener(&EnemyHpPanel::UpdatePosition));
	engine->Subscribe(ENUM_CLASS(EventID::EnemyHealthDecrease), MakeListener(&EnemyHpPanel::HealthDecrease));
	engine->Subscribe(ENUM_CLASS(EventID::EnemyHealthDecrease), MakeListener(&EnemyHpPanel::ActivePanel));
	engine->Subscribe(ENUM_CLASS(EventID::EnemyDead), MakeListener(&EnemyHpPanel::OwnerEnemyDead));

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_EnemyHpPanel");

	auto mtrlInstnace = sprite->GetMaterialInstance();
	mtrlInstnace->SetPass("InScreenUI_Pass");
	mtrlInstnace->SetFloat2("g_PanelSize", _float2{ 0.4f * 1.3f,0.07f * 1.65f });
	mtrlInstnace->SetFloat("g_Ratio", 1.f);
	mtrlInstnace->SetFloat("g_CustomAlpha", 1.f);

	m_Childrens.resize(ENUM_CLASS(Parts::Count));
	m_iOwnerID = desc->ownerID;

	if (FAILED(CreateChildren()))
		return E_FAIL;

	return S_OK;
}

void EnemyHpPanel::HealthDecrease(std::any param)
{
	ENEMY_HP_PANEL_PARAM panelParam = std::any_cast<ENEMY_HP_PANEL_PARAM>(param);
	if (panelParam.ownerID != m_iOwnerID)
		return;

	_float ratio = panelParam.ratio;

	/*hp decrease*/
	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = false;
		barParam.paused = true;
		barParam.pauseDuration = 0.3f;
		barParam.duration = 0.3f;
		barParam.targetRatio = ratio;

		Bar* health = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Decrease)]);
		health->MakeChange(barParam);
	}

	/*hp*/
	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = true;
		barParam.paused = false;
		barParam.pauseDuration = 0.f;
		barParam.duration = 0.001f;
		barParam.targetRatio = ratio;

		Bar* health = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Hp)]);
		health->MakeChange(barParam);
	}
}

void EnemyHpPanel::UpdatePosition(std::any param)
{
	ENEMY_HP_PANEL_PARAM panelParam = std::any_cast<ENEMY_HP_PANEL_PARAM>(param);
	if (panelParam.ownerID != m_iOwnerID)
		return;

	m_pTransform->SetPosition(panelParam.position);
}

void EnemyHpPanel::OwnerEnemyDead(std::any param)
{
	ENEMY_HP_PANEL_PARAM panelParam = std::any_cast<ENEMY_HP_PANEL_PARAM>(param);
	if (panelParam.ownerID != m_iOwnerID)
		return;

	SetDead();
}

void EnemyHpPanel::ActivePanel(std::any param)
{
	ENEMY_HP_PANEL_PARAM panelParam = std::any_cast<ENEMY_HP_PANEL_PARAM>(param);
	if (panelParam.ownerID != m_iOwnerID)
		return;

	m_IsActive = true;
	m_fElapsedTime = 0.f;
}

void EnemyHpPanel::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void EnemyHpPanel::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fDuration)
	{
		m_IsActive = false;
	}
}

void EnemyHpPanel::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT EnemyHpPanel::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	if (m_IsActive)
		return __super::ExtractRenderProxies(proxies);
	else
		return S_OK;
}

Object* EnemyHpPanel::Clone(InitDESC* arg)
{
	EnemyHpPanel* Instance = new EnemyHpPanel(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EnemyHpPanel::Free()
{
	__super::Free();
}

HRESULT EnemyHpPanel::CreateChildren()
{
	auto engine = EngineCore::GetInstance();

	/*add background*/
	{
		Bar::BAR_DESC desc{};
		desc.parent = this;
		desc.mtrlTag = "Mtrl_EnemyBackground";
		desc.scale = _float3{ m_fSizeX,m_fSizeY,1.f };
		
		Object* background = nullptr;
		background = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &desc);
		m_Childrens[ENUM_CLASS(Parts::Bar_Background)] = static_cast<UIObject*>(background);
		
		auto mtrlInstance = background->GetComponent<SpriteComponent>()->GetMaterialInstance();
		mtrlInstance->SetPass("InScreenUI_Pass");
		mtrlInstance->SetFloat2("g_PanelSize", _float2{ 0.4f * 1.2f,0.07f * 1.2f });
		mtrlInstance->SetFloat("g_Ratio", 1.f);
		mtrlInstance->SetFloat("g_CustomAlpha", 1.f);
	}
	/*add background*/
	{
		Bar::BAR_DESC desc{};
		desc.parent = this;
		desc.mtrlTag = "Mtrl_EnemyBackground2";
		desc.scale = _float3{ 249.f * 1.1f,8.f * 1.1f,1.f };
		
		Object* background = nullptr;
		background = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &desc);
		m_Childrens[ENUM_CLASS(Parts::Bar_Background2)] = static_cast<UIObject*>(background);
		
		auto mtrlInstance = background->GetComponent<SpriteComponent>()->GetMaterialInstance();
		mtrlInstance->SetPass("InScreenUI_Pass");
		mtrlInstance->SetFloat2("g_PanelSize", _float2{ 0.4f,0.07f * 0.3f });
		mtrlInstance->SetFloat("g_Ratio", 1.f);
		mtrlInstance->SetFloat("g_CustomAlpha", 1.f);
	}
	/*add hp decrease*/
	{
		Bar::BAR_DESC desc{};
		desc.parent = this;
		desc.mtrlTag = "Mtrl_EnemyHp0";
		desc.scale = _float3{ 249.f * 1.1f,8.f * 1.1f,1.f };

		Object* bar = nullptr;
		bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &desc);
		m_Childrens[ENUM_CLASS(Parts::Bar_Decrease)] = static_cast<UIObject*>(bar);

		auto mtrlInstance = bar->GetComponent<SpriteComponent>()->GetMaterialInstance();
		mtrlInstance->SetPass("InScreenUI_Pass");
		mtrlInstance->SetFloat2("g_PanelSize", _float2{ 0.4f,0.07f * 0.3f });
		mtrlInstance->SetFloat("g_Ratio", 1.f);
		mtrlInstance->SetFloat("g_CustomAlpha", 1.f);

	}
	/*add hp*/
	{
		Bar::BAR_DESC desc{};
		desc.parent = this;
		desc.mtrlTag = "Mtrl_EnemyHp1";
		desc.scale = _float3{ 249.f * 1.1f,8.f * 1.1f,1.f };

		Object* bar = nullptr;
		bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &desc);
		m_Childrens[ENUM_CLASS(Parts::Bar_Hp)] = static_cast<UIObject*>(bar);
		auto mtrlInstance = bar->GetComponent<SpriteComponent>()->GetMaterialInstance();

		mtrlInstance->SetPass("InScreenUI_Pass");
		mtrlInstance->SetFloat2("g_PanelSize", _float2{ 0.4f,0.07f * 0.3f });
		mtrlInstance->SetFloat("g_Ratio", 1.f);
		mtrlInstance->SetFloat("g_CustomAlpha", 1.f);
	}

	return S_OK;
}
