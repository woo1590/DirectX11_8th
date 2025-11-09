#include "pch.h"
#include "Prism.h"
#include "Random.h"
#include "Player.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

Prism::Prism()
	:Weapon()
{
}

Prism::Prism(const Prism& prototype)
	:Weapon(prototype)
{
}

Prism* Prism::Create()
{
	Prism* Instance = new Prism();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Prism::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	m_strInstanceTag = "Prism";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Prism::Initialize(InitDESC* arg)
{
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Weapon_Prism");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Weapon_Prism");

	model->ConnectAnimator();

	/*모델 세팅 이후에 무기 초기화 해야함*/
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	m_iFireLightBoneIndex = model->GetBoneIndex("FireLight");
	m_iNumMaxAmmo = 1;
	m_iNumCurrAmmo = m_iNumMaxAmmo;
	m_eWeaponID = WeaponID::Prism;

	ChangeState(&m_PrismIdle);

	return S_OK;
}

void Prism::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Prism::Update(_float dt)
{
	__super::Update(dt);
}

void Prism::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Prism::Fire()
{
	if (&m_PrismIdle == m_CurrState)
		ChangeState(&m_PrismFire);
}

Object* Prism::Clone(InitDESC* arg)
{
	Prism* Instance = new Prism(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Prism::Free()
{
	__super::Free();
}

void Prism::PrismIdle::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Idle), true);
}

void Prism::PrismIdle::Update(Object* object, _float dt)
{
}

void Prism::PrismIdle::TestForExit(Object* object)
{
}

void Prism::PrismFire::Enter(Object* object)
{
	m_IsReload = false;

	auto animator = object->GetComponent<AnimatorComponent>();
	_uint rand = EngineCore::GetInstance()->GetRandom()->get<_uint>(0, 2);
	
	if (0 == rand)
		animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire1), false, true);
	else if (1 == rand)
		animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire2), false, true);
	else
		animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire3), false, true);
	animator->SetPlaySpeedScale(2.f);

	auto engine = EngineCore::GetInstance();
	auto prism = static_cast<Prism*>(object);
	auto player = static_cast<Player*>(prism->m_pParent);

	/*for test*/
	_float4x4 boneMat = object->GetComponent<AnimatorComponent>()->GetCombinedMatrices()[prism->m_iFireLightBoneIndex];
	_float4x4 worldMat = object->GetComponent<TransformComponent>()->GetWorldMatrix();
	XMStoreFloat4x4(&worldMat, XMLoadFloat4x4(&boneMat) * XMLoadFloat4x4(&worldMat));
	
	_float3 aimPosition = player->GetAimPosition();
	_float3 position{};
	_vector positionV, scale, rot;
	XMMatrixDecompose(&scale, &rot, &positionV, XMLoadFloat4x4(&worldMat));
	XMStoreFloat3(&position, positionV);
	
	_float3 forward{};
	XMStoreFloat3(&forward, XMVector3Normalize(XMLoadFloat3(&aimPosition) - XMLoadFloat3(&position)));
	
	Object* prismProjectile = nullptr;
	Object::OBJECT_DESC desc{};
	desc.position = position;
	engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PrismProjectile", engine->GetCurrLevelID(), "Layer_Projectile", &desc, &prismProjectile);
	
	prismProjectile->GetComponent<TransformComponent>()->SetForward(forward);
	--prism->m_iNumCurrAmmo;

	engine->PublishEvent(ENUM_CLASS(EventID::CurrAmmoChange), prism->m_iNumCurrAmmo);
}

void Prism::PrismFire::Update(Object* object, _float dt)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (!m_IsReload && progress >= 0.5f)
	{
		auto engine = EngineCore::GetInstance();
		auto prism = static_cast<Prism*>(object);

		prism->m_iNumCurrAmmo = prism->m_iNumMaxAmmo;

		engine->PublishEvent(ENUM_CLASS(EventID::CurrAmmoChange), prism->m_iNumCurrAmmo);
		engine->PublishEvent(ENUM_CLASS(EventID::WeaponReload), prism->m_iNumMaxAmmo);

		m_IsReload = true;
	}
}

void Prism::PrismFire::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto engine = EngineCore::GetInstance();

		auto prism = static_cast<Prism*>(object);
		prism->ChangeState(&prism->m_PrismIdle);
	}
}
