#include "pch.h"
#include "BossLaserTrail.h"
#include "MaterialInstance.h"

//component
#include "TrailComponent.h"

BossLaserTrail::BossLaserTrail()
	:Object()
{
}

BossLaserTrail::BossLaserTrail(const BossLaserTrail& prototype)
	:Object(prototype)
{
}

BossLaserTrail* BossLaserTrail::Create()
{
	BossLaserTrail* Instance = new BossLaserTrail();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BossLaserTrail::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<TrailComponent>();
	m_eRenderGroup = RenderGroup::Blend;
	return S_OK;
}

HRESULT BossLaserTrail::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*trail*/
	TrailComponent::TRAIL_DESC trailDesc{};
	trailDesc.mode = TrailMode::Default;
	trailDesc.width = 10.f;
	auto trail = GetComponent<TrailComponent>();
	trail->Initialize(&trailDesc);
	trail->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_BossLaserTrail");

	trail->GetMaterialInstance()->SetPass("BossLaserTrail_Pass");

	return S_OK;
}

void BossLaserTrail::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossLaserTrail::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	_float width = std::abs(std::sinf(m_fElapsedTime * 3.f)) * 2.f + 5.f;

	GetComponent<TrailComponent>()->SetWidth(width);
}

void BossLaserTrail::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void BossLaserTrail::SetLaserPoints(_float3 p0, _float3 p1)
{
	auto trail = GetComponent<TrailComponent>();
	trail->AddPoints(p0, p1);
}

Object* BossLaserTrail::Clone(InitDESC* arg)
{
	BossLaserTrail* Instance = new BossLaserTrail(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossLaserTrail::Free()
{
	__super::Free();
}
