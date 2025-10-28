#include "MapEditorPCH.h"
#include "EnemySpawner.h"
#include "Bounding_AABB.h"
#include "PickingSystem.h"

//component
#include "ColliderComponent.h"

EnemySpawner::EnemySpawner()
	:Object()
{
}
EnemySpawner::EnemySpawner(const EnemySpawner& prototype)
	:Object(prototype),
	m_pPickingSystem(prototype.m_pPickingSystem)
{
	m_pPickingSystem->AddRef();
}

EnemySpawner* EnemySpawner::Create(PickingSystem* picking)
{
	EnemySpawner* Instance = new EnemySpawner();

	if (FAILED(Instance->Initialize_Prototype(picking)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EnemySpawner::Initialize_Prototype(PickingSystem* picking)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	m_pPickingSystem = picking;
	m_pPickingSystem->AddRef();

	return S_OK;
}

HRESULT EnemySpawner::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	Bounding_AABB::AABB_DESC aabbDesc{};
	aabbDesc.type = ColliderType::AABB;
	aabbDesc.center = _float3{ 0.f,10.f,0.f };
	aabbDesc.halfSize = _float3{ 10.f,10.f,10.f };

	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&aabbDesc);
	EngineCore::GetInstance()->RegisterCollider(collider);

	m_pPickingSystem->RegisterComponent(collider);

	return S_OK;
}

void EnemySpawner::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void EnemySpawner::Update(_float dt)
{
	__super::Update(dt);
}

void EnemySpawner::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

EnemySpawner::ENEMY_SPAWNER_DESC EnemySpawner::ExportDesc()
{
	ENEMY_SPAWNER_DESC desc{};

	desc.navCellIndices = m_AvailableNavCellIndices;
	desc.position = m_pTransform->GetPosition();
	desc.scale = m_pTransform->GetScale();
	desc.quaternion = m_pTransform->GetQuaternion();

	return desc;
}

Object* EnemySpawner::Clone(InitDESC* arg)
{
	EnemySpawner* Instance = new EnemySpawner(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);
	
	return Instance;
}

void EnemySpawner::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	m_pPickingSystem->UnRegisterComponent(GetComponent<ColliderComponent>());
	Safe_Release(m_pPickingSystem);

	__super::Free();
}

#ifdef USE_IMGUI
void EnemySpawner::RenderInspector()
{
	__super::RenderInspector();

	ImGui::SeparatorText("EnemySpanwer");
	for (_uint i = 0; i < m_AvailableNavCellIndices.size(); ++i)
	{
		ImGui::Text("Cell Index : %d", m_AvailableNavCellIndices[i]);
	}
}
#endif
