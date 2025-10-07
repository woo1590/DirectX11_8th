#include "EnginePCH.h"
#include "Socket.h"
#include "ModelComponent.h"

Socket::Socket()
	:PartObject()
{
}

Socket::Socket(const Socket& prototype)
	:PartObject(prototype)
{
}

Socket* Socket::Create()
{
	Socket* Instance = new Socket();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Socket::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Socket::Initialize(InitDESC* arg)
{
	SOCKET_DESC* desc = static_cast<SOCKET_DESC*>(arg);
	m_pParentModel = desc->parentModel;
	m_iBoneIndex = desc->boneIndex;
	m_useScale = desc->useScale;

	m_pParentModel->AddRef();

	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void Socket::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Socket::Update(_float dt)
{
	__super::Update(dt);

	_float4x4 socketMat = m_pParentModel->GetCombinedMatrixByIndex(m_iBoneIndex);
	_vector vScale, vRotation, vPosition;
	_float3 scale, position;
	_float4 rotation;

	/*Decompose Bone Matrix*/
	XMMatrixDecompose(&vScale, &vRotation, &vPosition, XMLoadFloat4x4(&socketMat));

	/*Set Scale*/
	if (m_useScale)
		XMStoreFloat3(&scale, vScale);
	else
		scale = _float3(1.f, 1.f, 1.f);
	m_pTransform->SetScale(scale);
	
	/*Set Rotation*/
	XMStoreFloat4(&rotation, XMQuaternionNormalize(vRotation));
	m_pTransform->SetQuaternion(rotation);

	/*Set Position*/
	XMStoreFloat3(&position, vPosition);
	m_pTransform->SetPosition(position);
}

void Socket::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* Socket::Clone(InitDESC* arg)
{
	Socket* Instance = new Socket(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Socket::Free()
{
	__super::Free();

	Safe_Release(m_pParentModel);
}
