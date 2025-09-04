#include "EnginePCH.h"
#include "InspectorPanel.h"
#include "Object.h"
#ifdef USE_IMGUI

InspectorPanel::InspectorPanel()
	:IPanel()
{
}

InspectorPanel* InspectorPanel::Create()
{
	InspectorPanel* Instance = new InspectorPanel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT InspectorPanel::Initialize()
{
	return S_OK;
}

void InspectorPanel::Draw(GUIState& state)
{
    static _bool show = true;
    if (!show) return;

    if (ImGui::Begin("Inspector", &show, ImGuiWindowFlags_MenuBar)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::TextUnformatted("Inspector");
            ImGui::EndMenuBar();
        }

        auto* object = state.pObject;
        if (!object) {
            ImGui::TextDisabled("No Object Selected");
            ImGui::End();
            return;
        }

        ImGui::Text("Object: %s", object->GetInstanceTag().c_str());
        ImGui::Separator();

        object->RenderInspector();
    }
    ImGui::End();
}

void InspectorPanel::Free()
{
	__super::Free();
}

#endif