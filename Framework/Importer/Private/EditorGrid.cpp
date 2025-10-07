#include "ImporterPCH.h"
#include "EditorGrid.h"
#include "SpriteComponent.h"

EditorGrid::EditorGrid()
	:Object()
{
}

EditorGrid::EditorGrid(const EditorGrid& prototype)
	:Object(prototype)
{
}

EditorGrid* EditorGrid::Create()
{
	EditorGrid* Instance = new EditorGrid();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EditorGrid::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "EditorGrid";
	m_eRenderGroup = RenderGroup::Blend;
	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT EditorGrid::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto sprite = GetComponent<SpriteComponent>();
	if (FAILED(sprite->Initialize(arg)))
		return E_FAIL;
	sprite->SetBuffer(ENUM_CLASS(LevelID::Import), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Import), "Mtrl_Grid");

	m_pTransform->Rotate(_float3(math::ToRadian(90.f), 0.f, 0.f));
	m_pTransform->SetScale(_float3(CHUNK_SIZE * CELL_SIZE, CHUNK_SIZE * CELL_SIZE, 1.f));
	m_pTransform->SetPosition(_float3(0.f, -100.f, 0.f));

	return S_OK;
}

void EditorGrid::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void EditorGrid::Update(_float dt)
{
	__super::Update(dt);
}

void EditorGrid::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* EditorGrid::Clone(InitDESC* arg)
{
	EditorGrid* Instance = new EditorGrid(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EditorGrid::Free()
{
	__super::Free();
}
