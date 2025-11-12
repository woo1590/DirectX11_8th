#include "pch.h"
#include "BossHpPanel.h"
#include "MaterialInstance.h"

//object
#include "BossIcon.h"
#include "Bar.h"

//component
#include "SpriteComponent.h"

BossHpPanel::BossHpPanel()
	:UIObject()
{
}

BossHpPanel::BossHpPanel(const BossHpPanel& prototype)
	:UIObject(prototype)
{
}

BossHpPanel* BossHpPanel::Create()
{
	BossHpPanel* Instance = new BossHpPanel();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BossHpPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT BossHpPanel::Initialize(InitDESC* arg)
{
	UIOBJECT_DESC desc{};
	desc.parent = nullptr;
	desc.sizeX = 720.f * 0.9f;
	desc.sizeY = 83.f * 0.9f;
	desc.x = WinSizeX * 0.5f;
	desc.y = WinSizeY * 0.1f;

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	engine->Subscribe(ENUM_CLASS(EventID::BossHealthDecrease), MakeListener(&BossHpPanel::HealthDecrease));
	
	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_BossHpPanel");

	m_Childrens.resize(ENUM_CLASS(Parts::Count));
	if (FAILED(CreateChildren()))
		return E_FAIL;

	return S_OK;
}

void BossHpPanel::HealthDecrease(std::any param)
{
	_float ratio = std::any_cast<_float>(param);

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
		barParam.duration = 0.1f;
		barParam.targetRatio = ratio;

		Bar* health = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Hp)]);
		health->MakeChange(barParam);
	}
}

void BossHpPanel::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossHpPanel::Update(_float dt)
{
	__super::Update(dt);
}

void BossHpPanel::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* BossHpPanel::Clone(InitDESC* arg)
{
	BossHpPanel* Instance = new BossHpPanel(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossHpPanel::Free()
{
	__super::Free();
}

HRESULT BossHpPanel::CreateChildren()
{
	auto engine = EngineCore::GetInstance();

	_float3 scale = m_pTransform->GetScale();

	/*bar hp background*/
	{
		Bar::BAR_DESC desc{};
		desc.parent = this;
		desc.priority = 1;
		desc.mtrlTag = "Mtrl_BossHp0";
		desc.position = _float3{ 0.f,m_fSizeY * -0.05f,0.f };
		desc.scale = _float3{ 653.f * 0.85f,21.f * 1.2f,1.f };

		Object* bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &desc);
		m_Childrens[ENUM_CLASS(Parts::Bar_Background)] = static_cast<UIObject*>(bar);
		bar->GetComponent<SpriteComponent>()->GetMaterialInstance()->SetFloat("g_Ratio", 0.99f);
	}
	/*bar hp decrease*/
	{
		Bar::BAR_DESC desc{};
		desc.parent = this;
		desc.priority = 1;
		desc.mtrlTag = "Mtrl_BossHp1";
		desc.position = _float3{ 0.f,m_fSizeY * -0.05f,0.f };
		desc.scale = _float3{ 653.f * 0.85f,21.f * 1.2f,1.f };

		Object* bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &desc);
		m_Childrens[ENUM_CLASS(Parts::Bar_Decrease)] = static_cast<UIObject*>(bar);
		bar->GetComponent<SpriteComponent>()->GetMaterialInstance()->SetFloat("g_Ratio", 0.99f);
		bar->GetComponent<SpriteComponent>()->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.7f);
	}
	/*bar hp*/
	{
		Bar::BAR_DESC desc{};
		desc.parent = this;
		desc.priority = 1;
		desc.mtrlTag = "Mtrl_BossHp2";
		desc.position = _float3{ 0.f,m_fSizeY * -0.05f,0.f };
		desc.scale = _float3{ 653.f * 0.85f,21.f * 1.2f,1.f };

		Object* bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &desc);
		m_Childrens[ENUM_CLASS(Parts::Bar_Hp)] = static_cast<UIObject*>(bar);
		bar->GetComponent<SpriteComponent>()->GetMaterialInstance()->SetFloat("g_Ratio", 0.99f);
	}
	/*boss icon*/
	{
		BossIcon::BOSS_ICON_DESC iconDesc{};
		iconDesc.parent = this;
		iconDesc.position = _float3{ -m_fSizeX * 0.5f,m_fSizeY * 0.2f,0.f };
		iconDesc.scale = _float3{ 219.f * 0.7f,208.f * 0.7f,1.f };

		Object* icon = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossIcon", &iconDesc);
		m_Childrens[ENUM_CLASS(Parts::Boss_Icon)] = static_cast<UIObject*>(icon);

	}

	return S_OK;
}

