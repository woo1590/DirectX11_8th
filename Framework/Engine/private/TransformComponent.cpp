#include "EnginePCH.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(Object* pOwner)
	:Component(pOwner)
{
}

TransformComponent::TransformComponent(const TransformComponent& prototype)
	:Component(prototype),
	m_Position(prototype.m_Position),
	m_Scale(prototype.m_Scale),
	m_Rotation(prototype.m_Rotation),
	m_Quaternion(prototype.m_Quaternion),
	m_Forward(prototype.m_Forward),
	m_Right(prototype.m_Right),
	m_Up(prototype.m_Up),
	m_WorldMatrix(prototype.m_WorldMatrix),
	m_isDirty(prototype.m_isDirty)
{

}

TransformComponent* TransformComponent::Create(Object* pOwner)
{
	TransformComponent* Instance = new TransformComponent(pOwner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TransformComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT TransformComponent::Initialize(InitDESC* arg)
{
	if (arg)
	{
		TransformDESC* desc = static_cast<TransformDESC*>(arg);

		m_Position = desc->position;
		m_Scale = desc->scale;
		m_Rotation = desc->rotation;

		return S_OK;
	}

	return S_OK;
}

void TransformComponent::Update(_float dt)
{
	if (m_isDirty)
	{
		_vector quat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&m_Rotation));
		
		_matrix s = XMMatrixScalingFromVector(XMLoadFloat3(&m_Scale));
		_matrix r = XMMatrixRotationQuaternion(quat);
		_matrix t = XMMatrixTranslationFromVector(XMLoadFloat3(&m_Position));

		XMStoreFloat4x4(&m_WorldMatrix, s * r * t);

		XMStoreFloat3(&m_Right, r.r[0]);
		XMStoreFloat3(&m_Up, r.r[1]);
		XMStoreFloat3(&m_Forward, r.r[2]);
		XMStoreFloat4(&m_Quaternion, quat);

		m_isDirty = false;
	}
}

void TransformComponent::SetForward(_float3 direction)
{
	_vector worldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector forward = XMVector3Normalize(XMLoadFloat3(&direction));
	_vector right = XMVector3Normalize(XMVector3Cross(worldUp, forward));
	_vector up = XMVector3Normalize(XMVector3Cross(forward, right));

	/*구면 좌표계를 통해서 pitch yaw roll 뽑아냄*/
	_float pitch = asin(std::clamp(-XMVectorGetY(forward), -1.f, 1.f));
	_float yaw = atan2(XMVectorGetX(forward), XMVectorGetZ(forward));
	_float roll = 0.f;

	/*짐벌락 생기면 roll 버림
	  버려도 되는 이유? 각 축에 대한 회전을 순차적으로 연산하지 않고 쿼터니언을 통해 회전행렬 계산 -> 결과는 제대로 나옴
	  but rotation에 저장되는 pitch, yaw, roll은 비정상적*/
	if (fabsf(cosf(pitch)) > 0.0001f) {
		roll = atan2f(XMVectorGetY(right), XMVectorGetY(up));
	}

	m_Rotation = { pitch,yaw,roll };

	m_isDirty = true;
}

void TransformComponent::Translate(_fvector velocity)
{
	XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + velocity);

	m_isDirty = true;
}

_matrix TransformComponent::GetWorldMatrix() const
{
	return XMLoadFloat4x4(&m_WorldMatrix);
}

_matrix TransformComponent::GetWorldMatrixInverse() const
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
}

Component* TransformComponent::Clone()
{
	TransformComponent* Instance = new TransformComponent(*this);

	return Instance;
}

void TransformComponent::Free()
{
	__super::Free();
}

