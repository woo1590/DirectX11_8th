#include "ImporterPCH.h"
#include "ImportLevel.h"
#include "EngineCore.h"
#include "VIBufferQuad.h"

//object
#include "EditorCamera.h"
#include "EditorLight.h"
#include "FBXModelObject.h"
#include "EditorGrid.h"

ImportLevel::ImportLevel()
	:Level()
{
}

ImportLevel* ImportLevel::Create()
{
	ImportLevel* Instance = new ImportLevel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ImportLevel::Initialize()
{
	/*loading 만들지 말고 여기서 바로 원본 생성*/
	auto engine = EngineCore::GetInstance();

	/*Load Shader*/	
	{
		if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_Grid.hlsl", "Shader_Grid",
			VTXTEX::elements, VTXTEX::numElement)))
			return E_FAIL;

		if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxMesh.hlsl", "Shader_VtxMesh",
				   VTXMESH::elements, VTXMESH::numElement)))
			return E_FAIL;

		if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxSkinnedMesh.hlsl", "Shader_VtxSkinnedMesh",
				   VTXSKINNEDMESH::elements, VTXSKINNEDMESH::numElement)))
			return E_FAIL;
	}

	/*Load Buffer*/
	{
		if (FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::Import), "Buffer_Quad", VIBufferQuad::Create())))
			return E_FAIL;
	}

	/*Load Material*/
	{
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Import), "../bin/grid.json", "Mtrl_Grid")))
			return E_FAIL;
	}

	/*Add Prototype*/
	{
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Import), "Prototype_Object_EditorCamera", EditorCamera::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Import), "Prototype_Object_FBXModelObject", FBXModelObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Import), "Prototype_Object_EditorLight", EditorLight::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Import), "Prototype_Object_EditorGrid", EditorGrid::Create())))
			return E_FAIL;
	}

	/*Add Object to Layer*/
	{
		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Import), "Prototype_Object_EditorCamera",
									 ENUM_CLASS(LevelID::Import), "Layer_ImportLevel")))
			return E_FAIL;

		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Import), "Prototype_Object_FBXModelObject",
									 ENUM_CLASS(LevelID::Import), "Layer_ImportLevel")))
			return E_FAIL;

		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Import), "Prototype_Object_FBXModelObject",
									 ENUM_CLASS(LevelID::Import), "Layer_ImportLevel")))
			return E_FAIL;

		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Import), "Prototype_Object_EditorGrid",
									 ENUM_CLASS(LevelID::Import), "Layer_ImportLevel")))
			return E_FAIL;

		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Import), "Prototype_Object_EditorLight",
									 ENUM_CLASS(LevelID::Import), "Layer_Light")))
			return E_FAIL;
	}

	return S_OK;
}

void ImportLevel::Update(_float dt)
{
}

HRESULT ImportLevel::Render()
{
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), L"FBXModel Importer");

	return S_OK;
}
