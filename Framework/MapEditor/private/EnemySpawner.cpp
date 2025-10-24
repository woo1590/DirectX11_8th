#include "MapEditorPCH.h"
#include "EnemySpawner.h"

EnemySpawner::EnemySpawner()
	:Object()
{
}
EnemySpawner::EnemySpawner(const EnemySpawner& prototype)
	:Object()
{

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

	return S_OK;
}

HRESULT EnemySpawner::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

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

Object* EnemySpawner::Clone(InitDESC* arg)
{
	EnemySpawner* Instance = new EnemySpawner(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);
	
	return Instance;
}

void EnemySpawner::Free()
{
	__super::Free();
}

#ifdef USE_IMGUI
void EnemySpawner::RenderInspector()
{
	__super::RenderInspector();
}
#endif
