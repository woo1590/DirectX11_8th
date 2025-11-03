#include "pch.h"
#include "Foundry.h"
#include "Player.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

Foundry::Foundry()
	:Weapon()
{
}

Foundry::Foundry(const Foundry& prototype)
	:Weapon(prototype)
{
}

Foundry* Foundry::Create()
{
	Foundry* Instance = new Foundry();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Foundry::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	m_strInstanceTag = "Foundry";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Foundry::Initialize(InitDESC* arg)
{
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Weapon_Foundry");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Weapon_Foundry");

	model->ConnectAnimator();

	m_iFireLightBoneIndex = model->GetBoneIndex("FireLight");

	/*모델 세팅 이후에 무기 초기화 해야함*/
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	m_iNumMaxAmmo = 8;
	m_iNumCurrAmmo = m_iNumMaxAmmo;
	m_eWeaponID = WeaponID::Foundry;

	ChangeState(&m_FoundryIdle);
	return S_OK;
}

void Foundry::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Foundry::Update(_float dt)
{
	__super::Update(dt);
}

void Foundry::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Foundry::Reload()
{
	if (m_CurrState == &m_FoundryIdle)
		ChangeState(&m_FoundryReload);
}

void Foundry::Fire()
{
	if (m_CurrState == &m_FoundryIdle)
	{
		if (m_iNumCurrAmmo > 0)
			ChangeState(&m_FoundryFire);
		else
			ChangeState(&m_FoundryReload);
	}
}

Object* Foundry::Clone(InitDESC* arg)
{
	Foundry* Instance = new Foundry(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Foundry::Free()
{
	__super::Free();
}

void Foundry::FoundryIdle::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Idle), true);
}

void Foundry::FoundryIdle::Update(Object* object, _float dt)
{
}

void Foundry::FoundryIdle::TestForExit(Object* object)
{
}

void Foundry::FoundryReload::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Reload), false, true);
}

void Foundry::FoundryReload::Update(Object* object, _float dt)
{
}

void Foundry::FoundryReload::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto foundry = static_cast<Foundry*>(object);
		foundry->ChangeState(&foundry->m_FoundryIdle);
		foundry->m_iNumCurrAmmo = foundry->m_iNumMaxAmmo;
	}
}

void Foundry::FoundryFire::Enter(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire), false, true);
	animator->SetPlaySpeedScale(1.2f);
	
	auto foundry = static_cast<Foundry*>(object);
	auto player = static_cast<Player*>(foundry->m_pParent);
	player->AddRecoil(4.f);

	/*for test*/
	_float4x4 boneMat = object->GetComponent<AnimatorComponent>()->GetCombinedMatrices()[foundry->m_iFireLightBoneIndex];
	_float4x4 worldMat = object->GetComponent<TransformComponent>()->GetWorldMatrix();
	XMStoreFloat4x4(&worldMat, XMLoadFloat4x4(&boneMat) * XMLoadFloat4x4(&worldMat));

	_float3 aimPosition = player->GetAimPosition();
	_float3 position{};
	_vector positionV, scale, rot;
	XMMatrixDecompose(&scale, &rot, &positionV, XMLoadFloat4x4(&worldMat));
	XMStoreFloat3(&position, positionV);

	_float3 forward{};
	XMStoreFloat3(&forward, XMVector3Normalize(XMLoadFloat3(&aimPosition) - XMLoadFloat3(&position)));

	Object* defaultBullet = nullptr;
	Object::OBJECT_DESC desc{};
	desc.scale = _float3{ 3.f,3.f,3.f };
	desc.position = position;
	engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Default_Bullet",engine->GetCurrLevelID(), "Layer_Projectile", &desc, &defaultBullet);

	defaultBullet->GetComponent<TransformComponent>()->SetForward(forward);
	--foundry->m_iNumCurrAmmo;
}

void Foundry::FoundryFire::Update(Object* object, _float dt)
{
}

void Foundry::FoundryFire::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto foundry = static_cast<Foundry*>(object);
		foundry->ChangeState(&foundry->m_FoundryIdle);
	}
}
