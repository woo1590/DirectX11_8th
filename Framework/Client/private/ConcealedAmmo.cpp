#include "pch.h"
#include "ConcealedAmmo.h"
#include "Player.h"

//object
#include "Socket.h"
#include "Dynamite.h"
#include "EffectContainer.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"

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
	auto engine = EngineCore::GetInstance();

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Weapon_ConcealedAmmo");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Weapon_ConcealedAmmo");

	model->ConnectAnimator();

	m_iFireLightBoneIndex = model->GetBoneIndex("FireLight");

	Socket::SOCKET_DESC socketDesc{};
	socketDesc.parentModel = model;
	socketDesc.boneIndex = m_iFireLightBoneIndex;
	socketDesc.useScale = false;
	Object* socket = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", &socketDesc);
	m_pMuzzleSocket = socket;
	m_pMuzzleSocket->GetComponent<TransformComponent>()->SetParent(m_pTransform);

	/*모델 세팅 이후에 무기 초기화 해야함*/
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	m_iNumMaxAmmo = 30;
	m_iNumCurrAmmo = m_iNumMaxAmmo;
	m_eWeaponID = WeaponID::ConcealedAmmo;
	ChangeState(&m_ConcealedAmmoIdle);

	return S_OK;
}

void ConcealedAmmo::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void ConcealedAmmo::Update(_float dt)
{
	__super::Update(dt);

	if (m_pMuzzleSocket)
		m_pMuzzleSocket->Update(dt);
}

void ConcealedAmmo::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void ConcealedAmmo::Idle()
{
	//ChangeState(&m_ConcealedAmmoIdle);
}

void ConcealedAmmo::Reload()
{
	if (&m_ConcealedAmmoIdle == m_CurrState)
		ChangeState(&m_ConcealedAmmoReload);
}

void ConcealedAmmo::Fire()
{
	if (&m_ConcealedAmmoIdle == m_CurrState)
	{
		if (m_iNumCurrAmmo > 0)
			ChangeState(&m_ConcealedAmmoFire);
		else
			ChangeState(&m_ConcealedAmmoReload);
	}
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

	Safe_Release(m_pMuzzleSocket);
}

void ConcealedAmmo::ConcealedAmmoIdle::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(1, true, false);

	auto ammo = static_cast<ConcealedAmmo*>(object);
	auto player = static_cast<Player*>(ammo->m_pParent);
	player->SetShotState(false);
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
	{
		auto engine = EngineCore::GetInstance();

		ammo->ChangeState(&ammo->m_ConcealedAmmoIdle);
		ammo->m_iNumCurrAmmo = ammo->m_iNumMaxAmmo;

		engine->PublishEvent(ENUM_CLASS(EventID::CurrAmmoChange), ammo->m_iNumCurrAmmo);
		engine->PublishEvent(ENUM_CLASS(EventID::WeaponReload), ammo->m_iNumMaxAmmo);
	}
}

void ConcealedAmmo::ConcealedAmmoFire::Enter(Engine::Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(0, false, true);
	animator->SetPlaySpeedScale(4.f);

	auto ammo = static_cast<ConcealedAmmo*>(object);
	auto player = static_cast<Player*>(ammo->m_pParent);
	player->SetShotState(true);
	player->AddRecoil(3.f);

	/*for test*/
	auto model = object->GetComponent<ModelComponent>();
	auto transform = object->GetComponent<TransformComponent>();

	_float4x4 boneMat = model->GetCombinedMatrixByIndex(ammo->m_iFireLightBoneIndex);
	_float4x4 worldMat = transform->GetWorldMatrix();
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
	--ammo->m_iNumCurrAmmo;

	engine->PublishEvent(ENUM_CLASS(EventID::CurrAmmoChange), ammo->m_iNumCurrAmmo);

	EffectContainer::EFFECT_CONTAINER_DESC effectDesc{};
	effectDesc.socketObject = ammo->m_pMuzzleSocket;
	engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_MuzzleRed", engine->GetCurrLevelID(), "Layer_Effect", &effectDesc);

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

	m_IsShot = false;
}

void ConcealedAmmo::ConcealedAmmoSkill::Update(Object* object, _float dt)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (!m_IsShot && progress >= 0.3f)
	{
		auto engine = EngineCore::GetInstance();

		auto ammo = static_cast<ConcealedAmmo*>(object);
		auto player = static_cast<Player*>(ammo->m_pParent);

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
		_float3 velocity{};
		XMStoreFloat3(&forward, XMVector3Normalize(XMLoadFloat3(&aimPosition) - XMLoadFloat3(&position)));
		XMStoreFloat3(&velocity, XMLoadFloat3(&forward) * 900.f);

		Object* dynamite = nullptr;
		Dynamite::DYNAMITE_DESC desc{};
		desc.scale = _float3{ 3.f,3.f,3.f };
		desc.position = position;
		desc.velocity = velocity;
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Dynamite", engine->GetCurrLevelID(), "Layer_Projectile", &desc, &dynamite);

		dynamite->GetComponent<TransformComponent>()->SetForward(forward);
		ammo->m_iNumCurrAmmo = 0;

		m_IsShot = true;
	}
}

void ConcealedAmmo::ConcealedAmmoSkill::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto ammo = static_cast<ConcealedAmmo*>(object);

	if (animator->IsFinished())
	{
		ammo->ChangeState(&ammo->m_ConcealedAmmoIdle);
		ammo->m_iNumCurrAmmo = ammo->m_iNumMaxAmmo;
	}
}
