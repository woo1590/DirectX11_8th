#include "MapEditorPCH.h"
#include "EditorLevel.h"
#include "EngineCore.h"
#include "MapEditorPanel.h"

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
