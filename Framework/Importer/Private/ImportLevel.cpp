#include "ImporterPCH.h"
#include "ImportLevel.h"
#include "EngineCore.h"

//object
#include "EditorCamera.h"
#include "EditorLight.h"
#include "FBXModelObject.h"

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
	if (FAILED(engine->LoadShaderFromFile(ENUM_CLASS(LevelID::Import), "../bin/shaderfiles/Shader_VtxMesh.hlsl", "Shader_VtxMesh",
			   VTXMESH::elements, VTXMESH::numElement)))
		return E_FAIL;

	/*Add Prototype*/
	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Import), "Prototype_Object_EditorCamera", EditorCamera::Create())))
		return E_FAIL;
	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Import), "Prototype_Object_FBXModelObject", FBXModelObject::Create())))
		return E_FAIL;
	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Import), "Prototype_Object_EditorLight", EditorLight::Create())))
		return E_FAIL;

	/*Add Object to Layer*/
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Import), "Prototype_Object_EditorCamera",
								 ENUM_CLASS(LevelID::Import), "Layer_ImportLevel")))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Import), "Prototype_Object_FBXModelObject",
								 ENUM_CLASS(LevelID::Import), "Layer_ImportLevel")))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Import), "Prototype_Object_EditorLight",
								 ENUM_CLASS(LevelID::Import), "Layer_Light")))
		return E_FAIL;

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
