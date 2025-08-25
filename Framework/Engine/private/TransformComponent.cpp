#include "EnginePCH.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(Object* pOwner)
	:Component(pOwner)
{
}

TransformComponent* TransformComponent::Create(Object* pOwner, InitDESC* arg)
{
	TransformComponent* Instance = new TransformComponent(pOwner);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
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
	if (isDirty)
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

		isDirty = false;
	}
}

void TransformComponent::SetForward(_float3 direction)
{
	_vector worldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector forward = XMVector3Normalize(XMLoadFloat3(&direction));
	_vector right = XMVector3Normalize(XMVector3Cross(worldUp, forward));
	_vector up = XMVector3Normalize(XMVector3Cross(forward, right));

	/*±¸¸é ÁÂÇ¥°è¸¦ ÅëÇØ¼­ pitch yaw roll »Ì¾Æ³¿*/
	_float pitch = asin(std::clamp(-XMVectorGetY(forward), -1.f, 1.f));
	_float yaw = atan2(XMVectorGetX(forward), XMVectorGetZ(forward));
	_float roll = 0.f;

	/*Áü¹ú¶ô »ý±â¸é roll ¹ö¸²*/
	if (fabsf(cosf(pitch)) > 0.0001f) {
		roll = atan2f(XMVectorGetY(right), XMVectorGetY(up));
	}

	m_Rotation = { pitch,yaw,roll };

	isDirty = true;
}

void TransformComponent::Translate(_fvector velocity)
{
	XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + velocity);
}

_matrix TransformComponent::GetWorldMatrix() const
{
	return XMLoadFloat4x4(&m_WorldMatrix);
}

_matrix TransformComponent::GetWorldMatrixInverse() const
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
}

void TransformComponent::Free()
{
	__super::Free();
}

