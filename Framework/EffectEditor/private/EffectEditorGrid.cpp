#include "pch.h"
#include "EffectEditorGrid.h"
#include "EngineCore.h"
#include "SpriteComponent.h"
#include "MaterialInstance.h"
#include "Shader.h"

EffectEditorGrid::EffectEditorGrid()
	:Object()
{
}

EffectEditorGrid::EffectEditorGrid(const EffectEditorGrid& prototype)
	:Object(prototype)
{
}

EffectEditorGrid* EffectEditorGrid::Create()
{
	EffectEditorGrid* Instance = new EffectEditorGrid();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EffectEditorGrid::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "EffectEditorGrid";
	m_eRenderGroup = RenderGroup::NonBlend;
	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT EffectEditorGrid::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto sprite = GetComponent<SpriteComponent>();
	if (FAILED(sprite->Initialize(arg)))
		return E_FAIL;
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Grid");

	m_pTransform->Rotate(_float3(math::ToRadian(90.f), 0.f, 0.f));
	m_pTransform->SetScale(_float3(CHUNK_SIZE * CELL_SIZE, CHUNK_SIZE * CELL_SIZE, 1.f));
	m_pTransform->SetPosition(_float3(0.f, -100.f, 0.f));

	return S_OK;
}

void EffectEditorGrid::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void EffectEditorGrid::Update(_float dt)
{
	__super::Update(dt);
}

void EffectEditorGrid::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* EffectEditorGrid::Clone(InitDESC* arg)
{
	EffectEditorGrid* Instance = new EffectEditorGrid(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EffectEditorGrid::Free()
{
	__super::Free();
}
