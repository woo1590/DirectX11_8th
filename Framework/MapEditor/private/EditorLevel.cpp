#include "MapEditorPCH.h"
#include "EditorLevel.h"
#include "EngineCore.h"
#include "MapEditorPanel.h"
#include "VIBufferQuad.h"
#include "PickingSystem.h"

//object
#include "MapEditorCamera.h"
#include "MapEditorLight.h"
#include "PreviewObject.h"
#include "NavMeshObject.h"
#include "Chunk.h"

EditorLevel::EditorLevel()
{
}

EditorLevel* EditorLevel::Create()
{
	EditorLevel* Instance = new EditorLevel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EditorLevel::Initialize()
{
	m_pPickingSystem = PickingSystem::Create();
	if (!m_pPickingSystem)
		return E_FAIL;

	if (FAILED(Initialize_DefaultResource()))
		return E_FAIL;

	if (FAILED(Initialize_LayerCamera("Layer_Camera")))
		return E_FAIL;

	if (FAILED(Initialize_LayerLight("Layer_Light")))
		return E_FAIL;

	if (FAILED(Initialize_LayerNavMeshObject("Layer_NavMeshObject")))
		return E_FAIL;

	if (FAILED(Initialize_LayerMapObject("Layer_MapObject")))
		return E_FAIL;

	auto editorPanel = MapEditorPanel::Create(m_pPickingSystem);
	if (!editorPanel)
	{
		MSG_BOX("Failed to create : MapEditorPanel");
		return E_FAIL;
	}
	editorPanel->LoadPrefabs("../bin/resource/prefabs.json");

	auto engine = EngineCore::GetInstance();
	engine->AddPanel(editorPanel);

	return S_OK;
}

void EditorLevel::Update(_float dt)
{
	__super::Update(dt);

	m_pPickingSystem->Update();
}

HRESULT EditorLevel::Render()
{
	__super::Render();

	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), L"Map Editor");

	return S_OK;
}

void EditorLevel::Free()
{
	__super::Free();
	Safe_Release(m_pPickingSystem);
}

HRESULT EditorLevel::Initialize_DefaultResource()
{
	auto engine = EngineCore::GetInstance();

	/*Load Shader*/
	{
		if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxMesh.hlsl", "Shader_VtxMesh",
			VTXMESH::elements, VTXMESH::numElement)))
			return E_FAIL;

		if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxSkinnedMesh.hlsl", "Shader_VtxSkinnedMesh",
			VTXSKINNEDMESH::elements, VTXSKINNEDMESH::numElement)))
			return E_FAIL;
	
		if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_Chunk.hlsl", "Shader_Chunk",
			VTXTEX::elements, VTXTEX::numElement)))
			return E_FAIL;
	}

	/*Load Model*/
	{
		if (FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::Editor), "Buffer_Quad", VIBufferQuad::Create())))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/stage1/stage1_wall0.model",
			"Model_Stage1_Wall0")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/stage1/stage1_wall1.model",
			"Model_Stage1_Wall1")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/stage1/stage1_wall2.model",
			"Model_Stage1_Wall2")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/stage1/stage1_wall3.model",
			"Model_Stage1_Wall3")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/stage1/stage1_wall4.model",
			"Model_Stage1_Wall4")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/stage1/stage1_wall5.model",
			"Model_Stage1_Wall5")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/stage1/stage1_wall6.model",
			"Model_Stage1_Wall6")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/stage1/stage1_wall7.model",
			"Model_Stage1_Wall7")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/stage1/stage1_wall8.model",
			"Model_Stage1_Wall8")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/stage1/stage1_wall9.model",
			"Model_Stage1_Wall9")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/boss/boss_stage.model",
			"Model_Boss_Stage")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Editor), "../bin/resource/models/map/floor_tile/floor_tile0.model",
			"Model_Floor_Tile0")))
			return E_FAIL;

	}
	
	/*Load Material*/
	{
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Editor), "../bin/resource/materials/chunk.json", "Mtrl_Chunk")))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT EditorLevel::Initialize_LayerLight(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	/*Add Prototype*/
	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Editor), "Prototype_Object_MapEditorLight", MapEditorLight::Create())))
		return E_FAIL;

	/*Add Object to layer*/
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Editor), "Prototype_Object_MapEditorLight", ENUM_CLASS(LevelID::Editor), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT EditorLevel::Initialize_LayerCamera(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	/*Add Prototype*/
	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Editor), "Prototype_Object_MapEditorCamera", MapEditorCamera::Create())))
		return E_FAIL;
	
	/*Add Object to layer*/
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Editor), "Prototype_Object_MapEditorCamera", ENUM_CLASS(LevelID::Editor), layerTag)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT EditorLevel::Initialize_LayerNavMeshObject(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	/*Add Prototype*/
	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Editor), "Prototype_Object_NavMeshObject", NavMeshObject::Create())))
		return E_FAIL;

	/*Add Object to layer*/
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Editor), "Prototype_Object_NavMeshObject", ENUM_CLASS(LevelID::Editor), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT EditorLevel::Initialize_LayerMapObject(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();
	/*Add Prototype*/
	{
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Editor), "Prototype_Object_Chunk", Chunk::Create(m_pPickingSystem))))
			return E_FAIL;	
	}
	
	/*Add Object to layer*/
	{
		Chunk::CHUNK_DESC desc{};
		desc.chunkPosition = _float3(0.f, 0.f, 0.f);
		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Editor), "Prototype_Object_Chunk", ENUM_CLASS(LevelID::Editor), layerTag, &desc)))
			return E_FAIL;
	}
	
	return S_OK;
}
