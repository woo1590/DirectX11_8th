#include "pch.h"
#include "Cameleon.h"
#include "EngineCore.h"
#include "Skeleton.h"
#include "Player.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

Cameleon::Cameleon()
	:Weapon()
{
}

Cameleon::Cameleon(const Cameleon& prototype)
	:Weapon(prototype)
{
}

Cameleon* Cameleon::Create()
{
	Cameleon* Instance = new Cameleon();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Cameleon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	m_strInstanceTag = "Cameleon";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Cameleon::Initialize(InitDESC* arg)
{
	auto engine = EngineCore::GetInstance();

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Weapon_Cameleon");

	/*animator*/
	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Weapon_Cameleon");

	model->ConnectAnimator();

	/*모델 세팅 이후에 무기 초기화 해야함*/
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	ChangeState(&m_CameleonIdle);
	m_iMuzzleBoneIndex = model->GetBoneIndex("muzzle");
	m_iNumMaxAmmo = 30;
	m_iNumCurrAmmo = m_iNumMaxAmmo;
	m_eWeaponID = WeaponID::Cameleon;


	return S_OK;
}

void Cameleon::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Cameleon::Update(_float dt)
{
	__super::Update(dt);

}

void Cameleon::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Cameleon::Idle()
{
	if (&m_CameleonReload != m_CurrState)
		ChangeState(&m_CameleonIdle);
}

void Cameleon::Reload()
{
	if (&m_CameleonIdle == m_CurrState)
		ChangeState(&m_CameleonReload);
}

void Cameleon::Fire()
{
	if (&m_CameleonIdle == m_CurrState)
		ChangeState(&m_CameleonFire);
}

Object* Cameleon::Clone(InitDESC* arg)
{
	Cameleon* Instance = new Cameleon(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Cameleon::Free()
{
	__super::Free();
}

void Cameleon::CameleonIdle::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->SetFadeDurtaion(0.2f);
	animator->ChangeAnimation(0, true, false);
}

void Cameleon::CameleonIdle::Update(Engine::Object* object, Engine::_float dt)
{
}

void Cameleon::CameleonIdle::TestForExit(Engine::Object* object)
{
}

void Cameleon::CameleonReload::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(1, false, false);
}

void Cameleon::CameleonReload::Update(Engine::Object* object, Engine::_float dt)
{
}

void Cameleon::CameleonReload::TestForExit(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto cameleon = static_cast<Cameleon*>(object);
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		object->ChangeState(&cameleon->m_CameleonIdle);
		cameleon->m_iNumCurrAmmo = cameleon->m_iNumMaxAmmo;
	}
}

void Cameleon::CameleonFire::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(3, true, false);
	animator->SetPlaySpeedScale(1.3f);

	m_IsShot = false;
}

void Cameleon::CameleonFire::Update(Engine::Object* object, Engine::_float dt)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (!m_IsShot && progress >= 0.5f)
	{
		auto engine = EngineCore::GetInstance();
		auto cameleon = static_cast<Cameleon*>(object);
		auto player = static_cast<Player*>(cameleon->m_pParent);
		player->SetShotState(true);

		/*for test*/
		_float4x4 boneMat = object->GetComponent<AnimatorComponent>()->GetCombinedMatrices()[cameleon->m_iMuzzleBoneIndex];
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
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Default_Bullet", engine->GetCurrLevelID(), "Layer_Projectile", &desc, &defaultBullet);

		defaultBullet->GetComponent<TransformComponent>()->SetForward(forward);
		--cameleon->m_iNumCurrAmmo;

		m_IsShot = true;
	}

	if (m_IsShot && progress >= 0.9f)
	{
		auto cameleon = static_cast<Cameleon*>(object);
		auto player = static_cast<Player*>(cameleon->m_pParent);
		m_IsShot = false;	
		player->SetShotState(false);
	}
}

void Cameleon::CameleonFire::TestForExit(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto cameleon = static_cast<Cameleon*>(object);

	if (animator->IsFinished())
		object->ChangeState(&cameleon->m_CameleonIdle);
	else if (cameleon->m_iNumCurrAmmo <= 0)
		cameleon->ChangeState(&cameleon->m_CameleonReload);
}
