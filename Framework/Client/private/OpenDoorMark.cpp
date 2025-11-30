#include "pch.h"
#include "OpenDoorMark.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

OpenDoorMark::OpenDoorMark()
	:Object()
{
}

OpenDoorMark::OpenDoorMark(const OpenDoorMark& prototype)
	:Object(prototype)
{
}

OpenDoorMark* OpenDoorMark::Create()
{
	OpenDoorMark* Instance = new OpenDoorMark();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT OpenDoorMark::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT OpenDoorMark::Initialize(InitDESC* arg)
{
	if(FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.isAnimated = false;
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_DoorMark");

	auto mtrlInstnace = sprite->GetMaterialInstance();
	mtrlInstnace->SetPass("InScreenUI_Pass");
	mtrlInstnace->SetFloat2("g_PanelSize", _float2{ 0.05f,0.05f });
	mtrlInstnace->SetFloat("g_Ratio", 1.f);
	mtrlInstnace->SetFloat("g_CustomAlpha", 1.f);

	/*door ring*/
	spriteDesc.fSpeed = 30.f;
	spriteDesc.iMaxFrameIndex = 16;
	spriteDesc.isAnimated = true;
	spriteDesc.isRepeat = false;
	m_pDoorRing = SpriteComponent::Create(this);
	m_pDoorRing->Initialize(&spriteDesc);
	m_pDoorRing->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pDoorRing->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_DoorRing");

	mtrlInstnace = m_pDoorRing->GetMaterialInstance();
	mtrlInstnace->SetPass("InScreenUI_Pass");
	mtrlInstnace->SetFloat2("g_PanelSize", _float2{ 0.4f,0.4f });
	mtrlInstnace->SetFloat("g_Ratio", 1.f);
	mtrlInstnace->SetFloat("g_CustomAlpha", 1.f);

	/*door arrow*/
	spriteDesc.fSpeed = 0.f;
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.isAnimated = false;
	spriteDesc.isRepeat = false;
	m_pDoorArrow = SpriteComponent::Create(this);
	m_pDoorArrow->Initialize(&spriteDesc);
	m_pDoorArrow->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pDoorArrow->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_DoorArrow");

	mtrlInstnace = m_pDoorArrow->GetMaterialInstance();
	mtrlInstnace->SetPass("DoorArrow_Pass");
	mtrlInstnace->SetFloat2("g_PanelSize", _float2{ 0.03f,0.03f });
	mtrlInstnace->SetFloat("g_ArrowOffsetY", m_fArrowOffsetY);
	mtrlInstnace->SetFloat("g_Ratio", 1.f);
	mtrlInstnace->SetFloat("g_CustomAlpha", 1.f);

	engine->Subscribe(ENUM_CLASS(EventID::PassDoor), MakeListener(&OpenDoorMark::Dead));

	return S_OK;
}

void OpenDoorMark::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void OpenDoorMark::Update(_float dt)
{
	__super::Update(dt);

	if (m_pDoorRing)
	{
		m_pDoorRing->Update(dt);
		if (m_pDoorRing->IsFinished())
			Safe_Release(m_pDoorRing);
	}

	if (m_pDoorArrow)
	{
		m_fElapsedTime += dt;
		_float offset = std::sin(m_fElapsedTime * 5.f) * 0.01f + m_fArrowOffsetY;

		auto mtrlInstance = m_pDoorArrow->GetMaterialInstance();
		mtrlInstance->SetFloat("g_ArrowOffsetY", offset);
	}
}

void OpenDoorMark::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT OpenDoorMark::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	__super::ExtractRenderProxies(proxies);

	if (m_pDoorRing)
		m_pDoorRing->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::Blend)]);

	if (m_pDoorArrow)
		m_pDoorArrow->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::Blend)]);

	return S_OK;
}

void OpenDoorMark::Dead(std::any param)
{
	SetDead();
}

Object* OpenDoorMark::Clone(InitDESC* arg)
{
	OpenDoorMark* Instance = new OpenDoorMark(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void OpenDoorMark::Free()
{
	__super::Free();

	Safe_Release(m_pDoorRing);
	Safe_Release(m_pDoorArrow);
}
