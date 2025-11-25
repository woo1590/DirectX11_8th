#include "pch.h"
#include "Hell.h"
#include "Socket.h"
#include "Player.h"
#include "DefaultBullet.h"
#include "Random.h"
#include "MaterialInstance.h"
#include "EffectContainer.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

Hell::Hell()
	:Weapon()
{
}

Hell::Hell(const Hell& prototype)
	:Weapon(prototype)
{
}

Hell* Hell::Create()
{
	Hell* Instance = new Hell();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Hell::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();

	m_strInstanceTag = "Hell";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Hell::Initialize(InitDESC* arg)
{
	auto engine = EngineCore::GetInstance();

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Weapon_Hell");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Weapon_Hell");

	model->ConnectAnimator();

	m_iFireLightBoneIndex = model->GetBoneIndex("muzzle");

	Socket::SOCKET_DESC socketDesc{};
	socketDesc.parentModel = model;
	socketDesc.boneIndex = m_iFireLightBoneIndex;
	socketDesc.useScale = false;
	Object* socket = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", &socketDesc);
	m_pMuzzleSocket = socket;
	//m_pMuzzleSocket->GetComponent<TransformComponent>()->SetPosition(_float3{ 0.f,-10.f,0.f });
	m_pMuzzleSocket->GetComponent<TransformComponent>()->SetParent(m_pTransform);

	/*모델 세팅 이후에 무기 초기화 해야함*/
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	m_pOutLineModel = ModelComponent::Create(this);
	m_pOutLineModel->SetModel(ENUM_CLASS(LevelID::Static), "Model_Weapon_Hell");
	m_pOutLineModel->Initialize(nullptr);
	auto outlineMtrlInstance = m_pOutLineModel->GetMaterialInstance();
	outlineMtrlInstance->SetPass("PlayerOutLine_Pass");
	outlineMtrlInstance->SetFloat4("g_OutLineColor", _float4(0.f, 0.f, 0.f, 1.f));
	outlineMtrlInstance->SetFloat("g_OutLineWidth", 0.025f);

	m_iNumMaxAmmo = 5;
	m_iNumCurrAmmo = m_iNumMaxAmmo;
	m_eWeaponID = WeaponID::Hell;

	ChangeState(&m_HellIdle);

	return S_OK;
}

void Hell::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Hell::Update(_float dt)
{
	__super::Update(dt);

	if (m_pMuzzleSocket)
		m_pMuzzleSocket->Update(dt);
}

void Hell::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Hell::Reload()
{
	if (m_CurrState == &m_HellIdle)
		ChangeState(&m_HellReloadStart);
}

void Hell::Fire()
{
	if (m_CurrState == &m_HellIdle || (m_CurrState == &m_HellReloadLoop && m_iNumCurrAmmo >= 1))
	{
		if (m_iNumCurrAmmo > 0)
			ChangeState(&m_HellFire1);
		else
			ChangeState(&m_HellReloadStart);
	}
}

void Hell::Skill()
{
}

void Hell::RenderInspector()
{
	__super::RenderInspector();

	if (m_pMuzzleSocket)
		m_pMuzzleSocket->RenderInspector();
}

Object* Hell::Clone(InitDESC* arg)
{
	Hell* Instance = new Hell(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Hell::Free()
{
	__super::Free();
	Safe_Release(m_pMuzzleSocket);
}

void Hell::HellIdle::Enter(Object* object)
{
	auto hell = static_cast<Hell*>(object);

	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Idle), true);

	auto player = static_cast<Player*>(hell->m_pParent);
	player->SetShotState(false);
}

void Hell::HellIdle::Update(Object* object, _float dt)
{
}

void Hell::HellIdle::TestForExit(Object* object)
{
}

void Hell::HellReloadStart::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::ReloadStart), false, true);
}

void Hell::HellReloadStart::Update(Object* object, _float dt)
{
}

void Hell::HellReloadStart::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	
	if (animator->IsFinished())
	{
		auto hell = static_cast<Hell*>(object);
		hell->ChangeState(&hell->m_HellReloadLoop);
	}
}

void Hell::HellReloadLoop::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::ReloadLoop),false, true);

	m_iReloadCount = 0;
}

void Hell::HellReloadLoop::Update(Object* object, _float dt)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (m_iReloadCount < 4 && animator->IsFinished())
	{
		auto engine = EngineCore::GetInstance();

		animator->ChangeAnimation(ENUM_CLASS(AnimationState::ReloadLoop), false, true);
		++m_iReloadCount;

		auto hell = static_cast<Hell*>(object);

		++hell->m_iNumCurrAmmo;
		engine->PublishEvent(ENUM_CLASS(EventID::CurrAmmoChange), hell->m_iNumCurrAmmo);
		engine->PublishEvent(ENUM_CLASS(EventID::WeaponReload), static_cast<_uint>(1));
	}
}

void Hell::HellReloadLoop::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (m_iReloadCount >= 4 && progress >= 0.85f)
	{
		auto engine = EngineCore::GetInstance();
		auto hell = static_cast<Hell*>(object);

		++hell->m_iNumCurrAmmo;
		engine->PublishEvent(ENUM_CLASS(EventID::CurrAmmoChange), hell->m_iNumCurrAmmo);
		engine->PublishEvent(ENUM_CLASS(EventID::WeaponReload), hell->m_iNumMaxAmmo);

		hell->ChangeState(&hell->m_HellReloadEnd);
	}
}

void Hell::HellReloadEnd::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::ReloadEnd), false, false);
}

void Hell::HellReloadEnd::Update(Object* object, _float dt)
{
}

void Hell::HellReloadEnd::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto hell = static_cast<Hell*>(object);
		hell->ChangeState(&hell->m_HellIdle);
	}
}

void Hell::HellFire1::Enter(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire1), false, true);

	auto hell = static_cast<Hell*>(object);
	auto player = static_cast<Player*>(hell->m_pParent);
	player->SetShotState(true);
	player->AddRecoil(7.f);

	/*for test*/
	_float4x4 boneMat = object->GetComponent<AnimatorComponent>()->GetCombinedMatrices()[hell->m_iFireLightBoneIndex];
	_float4x4 worldMat = object->GetComponent<TransformComponent>()->GetWorldMatrix();
	XMStoreFloat4x4(&worldMat, XMLoadFloat4x4(&boneMat) * XMLoadFloat4x4(&worldMat));

	_float3 aimPosition = player->GetAimPosition();
	_float3 position{};
	_vector positionV, scale, rot;
	XMMatrixDecompose(&scale, &rot, &positionV, XMLoadFloat4x4(&worldMat));
	XMStoreFloat3(&position, positionV);

	auto random = engine->GetRandom();
	for (_uint i = 0; i < 5; ++i)
	{
		_float3 forward{};
		_float3 randAngle{ random->get<_float>(math::ToRadian(-5.f),math::ToRadian(5.f)),
						   random->get<_float>(math::ToRadian(-5.f),math::ToRadian(5.f)),
						   random->get<_float>(math::ToRadian(-5.f),math::ToRadian(5.f))};
		_matrix rotMat = XMMatrixRotationRollPitchYaw(randAngle.x, randAngle.y, randAngle.z);
		XMStoreFloat3(&forward, XMVector3Normalize(XMLoadFloat3(&aimPosition) - XMLoadFloat3(&position)));
		XMStoreFloat3(&forward, XMVector3TransformNormal(XMLoadFloat3(&forward), rotMat));

		Object* defaultBullet = nullptr;
		DefaultBullet::DEFAULT_BULLET_DESC desc{};
		desc.scale = _float3{ 3.f,3.f,3.f };
		desc.position = position;
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Default_Bullet", engine->GetCurrLevelID(), "Layer_Projectile", &desc, &defaultBullet);

		defaultBullet->GetComponent<TransformComponent>()->SetForward(forward);
	}

	--hell->m_iNumCurrAmmo;
	engine->PublishEvent(ENUM_CLASS(EventID::CurrAmmoChange), hell->m_iNumCurrAmmo);

	EffectContainer::EFFECT_CONTAINER_DESC effectDesc{};
	effectDesc.socketObject = hell->m_pMuzzleSocket;
	effectDesc.scale = _float3{ 100.f,100.f,1.f };
	engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_MuzzleRed", engine->GetCurrLevelID(), "Layer_Effect", &effectDesc);
}

void Hell::HellFire1::Update(Object* object, _float dt)
{
}

void Hell::HellFire1::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto hell = static_cast<Hell*>(object);
		hell->ChangeState(&hell->m_HellFire2);
	}
}

void Hell::HellFire2::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire2), false, true);
}

void Hell::HellFire2::Update(Object* object, _float dt)
{
}

void Hell::HellFire2::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto hell = static_cast<Hell*>(object);
		hell->ChangeState(&hell->m_HellFire3);
	}
}

void Hell::HellFire3::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire3), false, true);
	animator->SetPlaySpeedScale(1.3f);
}

void Hell::HellFire3::Update(Object* object, _float dt)
{
}

void Hell::HellFire3::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto hell = static_cast<Hell*>(object);
		hell->ChangeState(&hell->m_HellIdle);
	}
}
