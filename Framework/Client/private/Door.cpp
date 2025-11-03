#include "pch.h"
#include "Door.h"
#include "Bounding_OBB.h"
#include "LoadingLevel.h"
#include "Command_ChangeLevel.h"

#include "ColliderComponent.h"
#include "Door_L.h"
#include "Door_R.h"

Door::Door()
	:ContainerObject()
{
}

Door::Door(const Door& prototype)
	:ContainerObject(prototype)
{
}

Door* Door::Create()
{
	Door* Instance = new Door();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Door::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	return S_OK;
}
HRESULT Door::Initialize(InitDESC* arg)
{
	if (FAILED(Object::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_OBB::OBB_DESC obbDesc{};
	obbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Door);
	obbDesc.type = ColliderType::OBB;
	obbDesc.center = _float3{ 0.f,-20.f,0.f };
	obbDesc.halfSize = _float3{ 10.f,20.f,10.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&obbDesc);
	engine->RegisterCollider(collider);

	m_iNumPartObjects = ENUM_CLASS(Parts::Count);
	m_PartObjects.resize(m_iNumPartObjects);
	if (FAILED(CreatePartObjects()))
		return E_FAIL;

	ChangeState(&m_DoorClosed);

	return S_OK;
}

void Door::ConnectNextStage(LevelID nextLevelID)
{
	m_IsConnectStage = true;
	m_eConnectStageID = nextLevelID;
}

void Door::Open()
{
	if (m_CurrState == &m_DoorClosed)
	{
		if (!m_IsConnectStage)
			ChangeState(&m_DoorOpening);
		else
			ChangeState(&m_DoorConnectNextStage);
	}
}

void Door::OnCollisionStay(ColliderComponent* otherCollider)
{
 	if (otherCollider->GetFilter() == ENUM_CLASS(ColliderFilter::Player))
	{
		if (&m_DoorConnectNextStage == m_CurrState)
		{
			auto engine = EngineCore::GetInstance();
			if (engine->IsKeyPressed('F'))
			{
				auto command = Command_ChangeLevel::Create(m_eConnectStageID);
				engine->RegisterCommand(command);  
			}
		}
	}
}

Object* Door::Clone(InitDESC* arg)
{
	Door* Instance = new Door(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Door::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

HRESULT Door::CreatePartObjects()
{
	/*add door l*/
	{
		Door_L::DOOR_L_DESC doorLDesc{};
		doorLDesc.parent = this;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Door_L", ENUM_CLASS(Parts::Door_L), &doorLDesc)))
			return E_FAIL;
	}
	/*add door r*/
	{
		Door_R::DOOR_R_DESC doorRDesc{};
		doorRDesc.parent = this;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Door_R", ENUM_CLASS(Parts::Door_R), &doorRDesc)))
			return E_FAIL;
	}

	return S_OK;
}

void Door::DoorOpen::Enter(Object* object)
{
	auto door = static_cast<Door*>(object);
	door->m_PartObjects[ENUM_CLASS(Parts::Door_L)]->GetComponent<TransformComponent>()->Rotate(_float3{ 0.f,math::ToRadian(-70.f),0.f });
	door->m_PartObjects[ENUM_CLASS(Parts::Door_R)]->GetComponent<TransformComponent>()->Rotate(_float3{ 0.f,math::ToRadian(70.f),0.f });
}

void Door::DoorOpen::Update(Object* object, _float t)
{
}

void Door::DoorOpen::TestForExit(Object* object)
{
}

void Door::DoorOpening::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
}

void Door::DoorOpening::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	if (m_fElapsedTime < m_fDuration)
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = std::clamp(t, 0.f, 1.f);

		_vector startRot, doorL_EndRot, doorR_EndRot;
		_float4 doorL_Result, doorR_Result;

		startRot = XMQuaternionRotationRollPitchYaw(0.f, 0.f, 0.f);
		doorL_EndRot = XMQuaternionRotationRollPitchYaw(0.f, math::ToRadian(-70.f), 0.f);
		doorR_EndRot = XMQuaternionRotationRollPitchYaw(0.f, math::ToRadian(70.f), 0.f);

		XMStoreFloat4(&doorL_Result, XMQuaternionSlerp(startRot, doorL_EndRot, t));
		XMStoreFloat4(&doorR_Result, XMQuaternionSlerp(startRot, doorR_EndRot, t));

		auto door = static_cast<Door*>(object);
		door->m_PartObjects[ENUM_CLASS(Parts::Door_L)]->GetComponent<TransformComponent>()->SetQuaternion(doorL_Result);
		door->m_PartObjects[ENUM_CLASS(Parts::Door_R)]->GetComponent<TransformComponent>()->SetQuaternion(doorR_Result);
	}

}

void Door::DoorOpening::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto door = static_cast<Door*>(object);
		door->ChangeState(&door->m_DoorOpen);
	}
}

void Door::DoorConnectNextStage::Enter(Object* object)
{
	/*ui 추가 등등*/
}

void Door::DoorConnectNextStage::Update(Object* object, _float dt)
{
}

void Door::DoorConnectNextStage::TestForExit(Object* object)
{
}
