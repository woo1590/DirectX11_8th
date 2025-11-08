#include "EnginePCH.h"
#include "TransformComponent.h"
#include "Object.h"

//component
#include "ModelComponent.h"
#include "NavigationComponent.h"
#include "RigidBodyComponent.h"

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
		TRANSFORM_DESC* desc = static_cast<TRANSFORM_DESC*>(arg);

		m_Position = desc->position;
		m_Scale = desc->scale;
		m_Quaternion = desc->quaternion;

		MakeDirty();
		return S_OK;
	}

	return S_OK;
}

void TransformComponent::SetQuaternion(_float4 quaternion)
{
	m_Quaternion = quaternion;

	MakeDirty();
}

void TransformComponent::SetForward(_float3 direction)
{
	_vector worldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector forward = XMVector3Normalize(XMLoadFloat3(&direction));
	_vector right = XMVector3Normalize(XMVector3Cross(worldUp, forward));
	_vector up = XMVector3Normalize(XMVector3Cross(forward, right));

	_float pitch = asin(std::clamp(-XMVectorGetY(forward), -1.f, 1.f));
	_float yaw = atan2(XMVectorGetX(forward), XMVectorGetZ(forward));
	_float roll = 0.f;

	if (fabsf(cosf(pitch)) > 0.0001f) {
		roll = atan2f(XMVectorGetY(right), XMVectorGetY(up));
	}

	XMStoreFloat4(&m_Quaternion, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));

	m_isDirty = true;
	MakeDirty();
}

void TransformComponent::SetParent(TransformComponent* parent)
{
	m_pParent = parent;
	m_pParent->m_Childrens.push_back(this);
}

void TransformComponent::RemoveChild(TransformComponent* child)
{
	auto iter = std::find(m_Childrens.begin(), m_Childrens.end(), child);

	if (iter != m_Childrens.end())
	{
		_uint index = std::distance(m_Childrens.begin(), iter);

		_uint lastIndex = m_Childrens.size() - 1;
		std::swap(m_Childrens[index], m_Childrens[lastIndex]);
		m_Childrens.pop_back();
	}
}

void TransformComponent::Translate(_fvector deltaMove)
{
	XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + deltaMove);
	
	MakeDirty();
}

void TransformComponent::Turn(_float deltaPitch, _float deltaYaw)
{
	m_PitchYaw.x += deltaPitch;
	m_PitchYaw.y += deltaYaw;

	m_PitchYaw.x = std::clamp(m_PitchYaw.x, math::ToRadian(-89.f), math::ToRadian(89.f));

	_vector quaternion = XMQuaternionIdentity();
	_vector worldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector yawQuternion = XMQuaternionRotationAxis(worldUp, m_PitchYaw.y);
	quaternion = XMQuaternionNormalize(XMQuaternionMultiply(quaternion, yawQuternion));

	_matrix yawRotMat = XMMatrixRotationQuaternion(quaternion);

	_vector right = XMVector3Normalize(yawRotMat.r[0]);
	_vector pitchQuternion = XMQuaternionRotationAxis(right, m_PitchYaw.x);

	quaternion = XMQuaternionNormalize(XMQuaternionMultiply(quaternion, pitchQuternion));

	XMStoreFloat4(&m_Quaternion, quaternion);

	MakeDirty();
}

void TransformComponent::Turn(_float3 deltaRadian)
{
	_vector quaternion = XMLoadFloat4(&m_Quaternion);

	_vector deltaQuaternion = XMQuaternionRotationRollPitchYaw(deltaRadian.x, deltaRadian.y, deltaRadian.z);

	quaternion = XMQuaternionNormalize(XMQuaternionMultiply(quaternion,deltaQuaternion));

	XMStoreFloat4(&m_Quaternion, quaternion);

	MakeDirty();
}

void TransformComponent::Rotate(_float3 euler)
{
	XMStoreFloat4(&m_Quaternion, XMQuaternionRotationRollPitchYaw(euler.x, euler.y, euler.z));

	MakeDirty();
}

_float3 TransformComponent::GetWorldPosition()
{
	if (m_pParent)
	{
		_float4x4 worldMat = GetWorldMatrix();
		_vector position, scale, rotation;
		_float3 worldPosition{};

		XMMatrixDecompose(&scale, &rotation, &position, XMLoadFloat4x4(&worldMat));
		XMStoreFloat3(&worldPosition, position);

		return worldPosition;
	}
	else
		return m_Position;
}

_float4 TransformComponent::GetWorldQuaternion()
{
	if (m_pParent)
	{
		_float4x4 worldMat = GetWorldMatrix();
		_vector position, scale, rotation;
		_float4 worldQuaternion{};

		XMMatrixDecompose(&scale, &rotation, &position, XMLoadFloat4x4(&worldMat));
		XMStoreFloat4(&worldQuaternion, rotation);

		return worldQuaternion;
	}
	else
		return m_Quaternion;
}

_float4x4 TransformComponent::GetWorldMatrix()
{
	ResolveDirty();

	return m_WorldMatrix;
}

_float4x4 TransformComponent::GetWorldMatrixInverse()
{
	ResolveDirty();

	return m_WorldMatrixInverse;
}

_float4x4 TransformComponent::GetLocalMatrix()
{
	ResolveDirty();

	_float4x4 localMat{};
	_matrix scaleMat = XMMatrixScalingFromVector(XMLoadFloat3(&m_Scale));
	_matrix rotMat = XMMatrixRotationQuaternion(XMLoadFloat4(&m_Quaternion));
	_matrix transMat = XMMatrixTranslationFromVector(XMLoadFloat3(&m_Position));
	
	XMStoreFloat4x4(&localMat, scaleMat * rotMat * transMat);

	return localMat;
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

#ifdef USE_IMGUI

void TransformComponent::RenderInspector()
{
	ImGui::PushID(this);

	if (!ImGui::CollapsingHeader("Transform",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		ImGui::Separator();

		_float3 degree{}, radian{}, deltaRad{};
		degree.x = math::ToDegree(m_Rotation.x);
		degree.y = math::ToDegree(m_Rotation.y);
		degree.z = math::ToDegree(m_Rotation.z);

		m_isDirty |= ImGui::DragFloat3("Position", &m_Position.x, 0.1f, -FLT_MAX, FLT_MAX);
		m_isDirty |= ImGui::DragFloat3("Scale", &m_Scale.x, 0.1f, -FLT_MAX, FLT_MAX);

		if (m_isDirty)
			MakeChildrenDirty();
		
		if (ImGui::DragFloat3("Rotation", &degree.x, 1.f, -FLT_MAX, FLT_MAX))
		{
			radian.x = math::ToRadian(degree.x);
			radian.y = math::ToRadian(degree.y);
			radian.z = math::ToRadian(degree.z);

			deltaRad.x = radian.x - m_Rotation.x;
			deltaRad.y = radian.y - m_Rotation.y;
			deltaRad.z = radian.z - m_Rotation.z;

			_vector quaternion = XMLoadFloat4(&m_Quaternion);
			_vector deltaQuternion = XMQuaternionRotationRollPitchYaw(deltaRad.x, deltaRad.y, deltaRad.z);
			_float4 newQuternion;
			XMStoreFloat4(&newQuternion, XMQuaternionNormalize(XMQuaternionMultiply(quaternion, deltaQuternion)));

			SetQuaternion(newQuternion);
			m_Rotation = radian;
		}

		ImGui::SeparatorText("Rotate 90");
		if (ImGui::Button("Pitch"))
			Turn(_float3(math::PI * 0.5f, 0.f, 0.f));
		if (ImGui::Button("Yaw"))
			Turn(_float3(0.f, math::PI * 0.5f, 0.f));
		if (ImGui::Button("Roll"))
			Turn(_float3(0.f, 0.f, math::PI * 0.5f));

	}

	ImGui::PopID();
}

#endif

void TransformComponent::ResolveDirty()
{
	if (m_isDirty)
	{
		_matrix scaleMat = XMMatrixScalingFromVector(XMLoadFloat3(&m_Scale));
		_matrix rotMat = XMMatrixRotationQuaternion(XMLoadFloat4(&m_Quaternion));
		_matrix transMat = XMMatrixTranslationFromVector(XMLoadFloat3(&m_Position));

		if (m_pParent)
		{
			_float4x4 parentMat = m_pParent->GetWorldMatrix();
			if (!m_UseParentScale)
			{
				_float3 right{ parentMat._11,parentMat._12,parentMat._13 };
				_float3 up{ parentMat._21,parentMat._22,parentMat._23 };
				_float3 look{ parentMat._31,parentMat._32,parentMat._33 };

				XMStoreFloat3(&right, XMVector3Normalize(XMLoadFloat3(&right)));
				XMStoreFloat3(&up, XMVector3Normalize(XMLoadFloat3(&up)));
				XMStoreFloat3(&look, XMVector3Normalize(XMLoadFloat3(&look)));
				
				parentMat._11 = right.x; 
				parentMat._12 = right.y; 
				parentMat._13 = right.z; 

				parentMat._21 = up.x; 
				parentMat._22 = up.y; 
				parentMat._23 = up.z; 

				parentMat._31 = look.x; 
				parentMat._32 = look.y; 
				parentMat._33 = look.z; 
			}

			XMStoreFloat4x4(&m_WorldMatrix, scaleMat * rotMat * transMat * XMLoadFloat4x4(&parentMat));
		}
		else
			XMStoreFloat4x4(&m_WorldMatrix, scaleMat * rotMat * transMat);

		XMStoreFloat4x4(&m_WorldMatrixInverse, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

		XMStoreFloat3(&m_Right, rotMat.r[0]);
		XMStoreFloat3(&m_Up, rotMat.r[1]);
		XMStoreFloat3(&m_Forward, rotMat.r[2]);

		_float3 euler{};
		euler = math::ToEuler(XMLoadFloat4(&m_Quaternion));
		m_Rotation = euler;

		m_isDirty = false;
	}
}

void TransformComponent::MakeDirty()
{
	m_isDirty = true;
	MakeChildrenDirty();
}

void TransformComponent::MakeChildrenDirty()
{
	for (const auto& child : m_Childrens)
	{
		if (child)
		{
			child->m_isDirty = true;
			child->MakeChildrenDirty();
		}
	}
}

_vector TransformComponent::RemoveRoll(_fvector quaternion)
{
	_vector worldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_matrix rotMat = XMMatrixRotationQuaternion(quaternion);
	_vector forward = XMVector3Normalize(rotMat.r[2]);
	_vector up = XMVector3Normalize(rotMat.r[1]);

	_vector crossUp = XMVector3Cross(up, worldUp);

	_float rollNum = XMVectorGetX(XMVector3Dot(crossUp, forward));
	_float rollDen = XMVectorGetX(XMVector3Dot(up, worldUp));
	_float rollAngle = std::atan2(rollNum, rollDen);

	_vector rollFixQuaternion = XMQuaternionNormalize(XMQuaternionRotationAxis(forward, -rollAngle));

	return XMQuaternionNormalize(XMQuaternionMultiply(quaternion, rollFixQuaternion));
}

