#include "ImporterPCH.h"
#include "FBXCamLoaderComponent.h"

FBXCamLoaderComponent::FBXCamLoaderComponent(Object* owner)
	:Component(owner)
{
}

FBXCamLoaderComponent::FBXCamLoaderComponent(const FBXCamLoaderComponent& prototype)
	:Component(prototype)
{
}

FBXCamLoaderComponent* FBXCamLoaderComponent::Create(Object* owner)
{
	FBXCamLoaderComponent* Instance = new FBXCamLoaderComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FBXCamLoaderComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT FBXCamLoaderComponent::Initialize(InitDESC* desc)
{
	return S_OK;
}

void FBXCamLoaderComponent::Free()
{
	__super::Free();
}

#ifdef USE_IMGUI

void FBXCamLoaderComponent::RenderInspector()
{
	namespace fs = std::filesystem;

	static _string defaultPath = "../Bin/fbx/";
	static _string openedFile{};
	static _string savedFileName{};
	static _string currDir{};

	ImGui::PushID(this);

	if (!ImGui::CollapsingHeader("FBXLoader",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		ImportInspector(openedFile);

		if (m_isLoaded)
			ExportInspector(savedFileName);
	}

	ImGui::PopID();
}

void FBXCamLoaderComponent::ImportInspector(_string opendFile)
{
}

void FBXCamLoaderComponent::ExportInspector(_string savedFileName)
{
}

#endif