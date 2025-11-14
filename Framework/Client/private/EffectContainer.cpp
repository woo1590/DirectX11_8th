#include "pch.h"
#include "EffectContainer.h"
#include "SpriteEffect.h"

EffectContainer::EffectContainer()
	:ContainerObject()
{
}

EffectContainer::EffectContainer(const EffectContainer& prototype)
	:ContainerObject(prototype)
{
	PartObject::PART_OBJECT_DESC desc{};
	desc.parent = this;

	for (auto& effect : prototype.m_PartObjects)
	{
		m_PartObjects.push_back(static_cast<PartObject*>(effect->Clone(&desc)));
	}

	m_fDuration = prototype.m_fDuration;
	m_iNumPartObjects = m_PartObjects.size();
}

EffectContainer* EffectContainer::Create(const _string& filePath)
{
	EffectContainer* Instance = new EffectContainer();

	if (FAILED(Instance->Initialize_Prototype(filePath)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EffectContainer::Initialize_Prototype(const _string& filePath)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	using namespace nlohmann;
	namespace fs = std::filesystem;

	auto engine = EngineCore::GetInstance();
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		MSG_BOX("Failed to load : Effect Prefab");
		return E_FAIL;
	}

	_float3 position{};
	_float3 scale{};
	_float4 quaternion{};

	ordered_json prefab = json::parse(file);
	m_iNumPartObjects = prefab.at("num_node").get<_uint>();
	m_fDuration = prefab.at("duration").get<_float>();

	position.x = prefab.at("position").at("x").get<_float>();
	position.y = prefab.at("position").at("y").get<_float>();
	position.z = prefab.at("position").at("z").get<_float>();

	scale.x = prefab.at("scale").at("x").get<_float>();
	scale.y = prefab.at("scale").at("y").get<_float>();
	scale.z = prefab.at("scale").at("z").get<_float>();

	quaternion.x = prefab.at("quaternion").at("x").get<_float>();
	quaternion.y = prefab.at("quaternion").at("y").get<_float>();
	quaternion.z = prefab.at("quaternion").at("z").get<_float>();
	quaternion.w = prefab.at("quaternion").at("w").get<_float>();

	for (auto& node : prefab["nodes"])
	{
		_string type = node.at("type").get<_string>();
		if ("sprite" == type)
		{
			auto spriteEffect = SpriteEffect::Create(node, filePath);
			m_PartObjects.push_back(spriteEffect);
		}
	}

	m_pTransform->SetPosition(position);
	m_pTransform->SetScale(scale);
	m_pTransform->SetQuaternion(quaternion);

	return S_OK;
}

HRESULT EffectContainer::Initialize(InitDESC* arg)
{
	EFFECT_CONTAINER_DESC* desc = static_cast<EFFECT_CONTAINER_DESC*>(arg);

	//if (FAILED(Object::Initialize(desc)))
	//	return E_FAIL;

	if (desc->socketObject)
		m_pParentSocket = desc->socketObject;

	m_pTransform->SetPosition(desc->position);
	m_pTransform->SetForward(desc->forward);

	return S_OK;
}

void EffectContainer::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void EffectContainer::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fDuration)
	{
		//SetDead();
	}
	else
	{
		if (m_pParentSocket)
		{
			auto socketTransform = m_pParentSocket->GetComponent<TransformComponent>();
			_float4x4 socketWorldMat = socketTransform->GetWorldMatrix();

			_float3 position{ socketWorldMat._41,socketWorldMat._42,socketWorldMat._43 };
			m_pTransform->SetPosition(position);
		}
	}
}

void EffectContainer::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* EffectContainer::Clone(InitDESC* arg)
{
	EffectContainer* Instance = new EffectContainer(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EffectContainer::Free()
{
	__super::Free();
}
