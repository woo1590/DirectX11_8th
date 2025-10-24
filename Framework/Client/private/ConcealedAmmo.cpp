#include "pch.h"
#include "ConcealedAmmo.h"
#include "Player.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

ConcealedAmmo::ConcealedAmmo()
	:Weapon()
{
}

ConcealedAmmo::ConcealedAmmo(const ConcealedAmmo& prototype)
	:Weapon(prototype)
{
}

ConcealedAmmo* ConcealedAmmo::Create()
{
	ConcealedAmmo* Instance = new ConcealedAmmo();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ConcealedAmmo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	m_strInstanceTag = "ConcealedAmmo";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT ConcealedAmmo::Initialize(InitDESC* arg)
{
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Weapon_ConcealedAmmo");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Weapon_ConcealedAmmo");

	model->ConnectAnimator();

	m_iFireLightBoneIndex = model->GetBoneIndex("FireLight");

	ChangeState(&m_ConcealedAmmoIdle);

	/*모델 세팅 이후에 무기 초기화 해야함*/
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void ConcealedAmmo::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void ConcealedAmmo::Update(_float dt)
{
	__super::Update(dt);

}

void ConcealedAmmo::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void ConcealedAmmo::Idle()
{
	ChangeState(&m_ConcealedAmmoIdle);
}

void ConcealedAmmo::Reload()
{
	if (&m_ConcealedAmmoIdle == m_CurrState)
		ChangeState(&m_ConcealedAmmoReload);
}

void ConcealedAmmo::Fire()
{
	if (&m_ConcealedAmmoIdle == m_CurrState)
		ChangeState(&m_ConcealedAmmoFire);
}

void ConcealedAmmo::Skill()
{
	if (&m_ConcealedAmmoIdle == m_CurrState)
		ChangeState(&m_ConcealedAmmoSkill);
}

Object* ConcealedAmmo::Clone(InitDESC* arg)
{
	ConcealedAmmo* Instance = new ConcealedAmmo(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void ConcealedAmmo::Free()
{
	__super::Free();
}

void ConcealedAmmo::ConcealedAmmoIdle::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(1, true, false);
}

void ConcealedAmmo::ConcealedAmmoIdle::Update(Engine::Object* object, Engine::_float dt)
{
}

void ConcealedAmmo::ConcealedAmmoIdle::TestForExit(Engine::Object* object)
{
}

void ConcealedAmmo::ConcealedAmmoReload::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(2, false, true);
	animator->SetPlaySpeedScale(1.3f);
}

void ConcealedAmmo::ConcealedAmmoReload::Update(Engine::Object* object, Engine::_float dt)
{
}

void ConcealedAmmo::ConcealedAmmoReload::TestForExit(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto ammo = static_cast<ConcealedAmmo*>(object);

	if (animator->IsFinished())
		ammo->ChangeState(&ammo->m_ConcealedAmmoIdle);
}

void ConcealedAmmo::ConcealedAmmoFire::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(0, false, true);
	animator->SetPlaySpeedScale(4.f);

	auto ammo = static_cast<ConcealedAmmo*>(object);
	auto player = static_cast<Player*>(ammo->m_pParent);
	player->AddRecoil(3.f);

	/*for test*/
	_float4x4 boneMat = object->GetComponent<AnimatorComponent>()->GetCombinedMatrices()[ammo->m_iFireLightBoneIndex];
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
	EngineCore::GetInstance()->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Default_Bullet", ENUM_CLASS(LevelID::GamePlay), "Layer_Projectile", &desc, &defaultBullet);

	defaultBullet->GetComponent<TransformComponent>()->SetForward(forward);
}

void ConcealedAmmo::ConcealedAmmoFire::Update(Engine::Object* object, Engine::_float dt)
{
	auto animator = object->GetComponent<AnimatorComponent>();
}

void ConcealedAmmo::ConcealedAmmoFire::TestForExit(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto blast = static_cast<ConcealedAmmo*>(object);

	if (animator->IsFinished())
		blast->ChangeState(&blast->m_ConcealedAmmoIdle);
}

void ConcealedAmmo::ConcealedAmmoSkill::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(4, false, true);
}

void ConcealedAmmo::ConcealedAmmoSkill::Update(Object* object, _float dt)
{
}

void ConcealedAmmo::ConcealedAmmoSkill::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto blast = static_cast<ConcealedAmmo*>(object);

	if (animator->IsFinished())
		blast->ChangeState(&blast->m_ConcealedAmmoIdle);
}
