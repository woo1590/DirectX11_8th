#include "pch.h"
#include "PrismTrail.h"
#include "MaterialInstance.h"

//component
#include "TrailComponent.h"

PrismTrail::PrismTrail()
	:Object()
{
}

PrismTrail::PrismTrail(const PrismTrail& prototype)
	:Object(prototype)
{
}

PrismTrail* PrismTrail::Create()
{
	PrismTrail* Instance = new PrismTrail();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PrismTrail::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<TrailComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT PrismTrail::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*trail*/	
	TrailComponent::TRAIL_DESC trailDesc{};
	trailDesc.mode = TrailMode::Ribbon;
	trailDesc.maxLifeTime = 0.2f;
	trailDesc.minDistance = 10.f;
	trailDesc.width = 5.f;
	trailDesc.numMaxPoints = 30;

	auto trail = GetComponent<TrailComponent>();
	trail->Initialize(&trailDesc);

	trail->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_TrailPrism");
	trail->GetMaterialInstance()->SetPass("Ribbon_Pass");

	return S_OK;
}

void PrismTrail::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void PrismTrail::Update(_float dt)
{
	__super::Update(dt);
}

void PrismTrail::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* PrismTrail::Clone(InitDESC* arg)
{
	PrismTrail* Instance = new PrismTrail(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void PrismTrail::Free()
{
	__super::Free();
}
