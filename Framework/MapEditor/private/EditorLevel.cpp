#include "MapEditorPCH.h"
#include "EditorLevel.h"
#include "EngineCore.h"
#include "MapEditorPanel.h"

//object
#include "MapEditorCamera.h"
#include "MapEditorLight.h"

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
	auto editorPanel = MapEditorPanel::Create();
	if (!editorPanel)
	{
		MSG_BOX("Failed to create : MapEditorPanel");
		return E_FAIL;
	}

	EngineCore::GetInstance()->AddPanel(editorPanel);

	if (FAILED(Initialize_LayerCamera("Layer_Camera")))
		return E_FAIL;

	if (FAILED(Initialize_LayerLight("Layer_Light")))
		return E_FAIL;

	return S_OK;
}

void EditorLevel::Update(_float dt)
{
	__super::Update(dt);
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
