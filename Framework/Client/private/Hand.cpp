#include "pch.h"
#include "Hand.h"
#include "Player.h"

//component
#include "TransformComponent.h"
#include "AnimatorComponent.h"
#include "ModelComponent.h"

#include "BobbingController.h"

Hand::Hand()
	:PartObject()
{
}

Hand::Hand(const Hand& prototype)
	:PartObject(prototype)
{
}

Hand* Hand::Create()
{
	Hand* Instance = new Hand();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Hand::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<AnimatorComponent>();
	AddComponent<ModelComponent>();
	m_strInstanceTag = "Hand";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Hand::Initialize(InitDESC* arg)
{
	HAND_DESC* desc = static_cast<HAND_DESC*>(arg);

	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	m_pTransform->SetPosition(_float3{ -0.1f,0.f,-1.2f });

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_PlayerHand");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_PlayerHand");

	model->ConnectAnimator();
	animator->ChangeAnimation(0);

	XMStoreFloat4(&m_TargetRotation, XMQuaternionIdentity());
	XMStoreFloat4(&m_CurrRotation, XMQuaternionIdentity());

	m_pBobbingController = BobbingController::Create();

	ChangeState(&m_HandIdle);

	return S_OK;
}

void Hand::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Hand::Update(_float dt)
{
	__super::Update(dt); 

	UpdateSway(dt);

	_float3 posAdd{};
	_float3 rotAdd{};
	m_pBobbingController->Update(dt, posAdd, rotAdd);

	_float4x4 additive{};
	_matrix transMat = XMMatrixTranslation(posAdd.x, posAdd.y, posAdd.z);
	_matrix rotMat = XMMatrixRotationRollPitchYaw(rotAdd.x, rotAdd.y, rotAdd.z);

	XMStoreFloat4x4(&additive, rotMat * transMat);
	GetComponent<AnimatorComponent>()->SetAdditiveMatrix(additive, 1);

	
}

void Hand::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Hand::Idle()
{
	if (m_CurrState == &m_HandWalk)
		ChangeState(&m_HandIdle);
}

void Hand::Walk()
{
	if (m_CurrState == &m_HandIdle)
		ChangeState(&m_HandWalk);
}

void Hand::StartJump()
{
	m_pBobbingController->StartJump();
}

void Hand::EndJump()
{
	m_pBobbingController->EndJump();
}

void Hand::AddRecoil(_float power)
{
	m_pBobbingController->AddRecoil(power);
}

void Hand::SetSwayTargetRoation(_float yaw)
{
	yaw = std::clamp(yaw, -m_fMaxSwayAmount, m_fMaxSwayAmount);

	XMStoreFloat4(&m_TargetRotation, XMQuaternionRotationRollPitchYaw(0.f, yaw, 0.f));
}

void Hand::SwayEnable()
{
	if (!m_EnableSway)
	{
		m_EnableSway = true;
		m_CurrRotation = _float4(0.f, 0.f, 0.f, 1.f);
		m_TargetRotation = _float4{ 0.f,0.f,0.f,1.f };
	}
}

void Hand::SwayDisable()
{
	m_EnableSway = false;
	m_CurrRotation = _float4(0.f, 0.f, 0.f, 1.f);
	m_TargetRotation = _float4{ 0.f,0.f,0.f,1.f };
}

Object* Hand::Clone(InitDESC* arg)
{
	Hand* Instance = new Hand(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Hand::Free()
{
	__super::Free();

	Safe_Release(m_pBobbingController);
}

void Hand::UpdateSway(_float dt)
{
	if (!m_EnableSway)
		return;

	_vector currRotation = XMLoadFloat4(&m_CurrRotation);
	_vector targetRotation = XMLoadFloat4(&m_TargetRotation);

	currRotation = XMQuaternionSlerp(currRotation, targetRotation, m_fSmoothAmount * dt);

	XMStoreFloat4(&m_CurrRotation, currRotation);
	m_pTransform->SetQuaternion(m_CurrRotation);
}

void Hand::HandIdle::Enter(Object* object)
{
	auto hand = static_cast<Hand*>(object);

	hand->m_pBobbingController->OnIdle();
}

void Hand::HandWalk::Enter(Object* object)
{
	auto hand = static_cast<Hand*>(object);

	hand->m_pBobbingController->OnWalk();
}
