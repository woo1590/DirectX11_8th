#include "EnginePCH.h"
#include "OutlinerPanel.h"
#include "ObjectManager.h"
#include "LevelManager.h"
#include "Object.h"
#include "Level.h"
#include "Layer.h"


#ifdef USE_IMGUI

OutlinerPanel::OutlinerPanel()
	:IPanel()
{
}

OutlinerPanel* OutlinerPanel::Create()
{
	OutlinerPanel* Instance = new OutlinerPanel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT OutlinerPanel::Initialize()
{
	return S_OK;
}

void OutlinerPanel::Draw(GUIState& state)
{
    static _bool show = true;
    if (!show) return;

    const ImGuiViewport* vp = ImGui::GetMainViewport();
    ImVec2 pad = ImVec2(12, 12);

    ImVec2 pos = ImVec2(0, 0);
    ImVec2 size = ImVec2(vp->WorkSize.x * 0.2f, vp->WorkSize.y);
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0, 0));
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Outliner", &show, ImGuiWindowFlags_MenuBar)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::TextUnformatted("Outliner");
            ImGui::EndMenuBar();
        }

        static ImGuiTextFilter filter;
        filter.Draw("Filter", 180.0f);

        _uint levelID = state.pLevelManager->GetCurrLevelID();
        const auto& layers = state.pObjectManager->GetLayers(levelID);
        const auto& staticlayers = state.pObjectManager->GetLayers(0);

        for (const auto& layer : staticlayers) {
            if (!filter.IsActive() || filter.PassFilter(layer.first.c_str())) {
                ImGui::PushID(layer.second);

                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
                _bool open = ImGui::TreeNodeEx("Layer", flags, "Layer: %s", layer.first.c_str());

                if (ImGui::IsItemClicked()) {
                    state.pLayer = layer.second;
                    state.pObject = nullptr;
                }

                if (open) {
                    for (auto* object : layer.second->GetObjects()) {
                        if (!object) continue;
                        if (filter.IsActive() && !filter.PassFilter(object->GetInstanceTag().c_str()))
                            continue;

                        ImGui::PushID(object);
                        _bool selected = (state.pObject == object);
                        if (ImGui::Selectable(object->GetInstanceTag().c_str(), selected)) {
                            state.pObject = object;
                            state.pLayer = layer.second;
                        }
                        ImGui::PopID();
                    }
                    ImGui::TreePop();
                }

                ImGui::PopID();
            }
        }
        for (const auto& layer : layers) {
            if (!filter.IsActive() || filter.PassFilter(layer.first.c_str())) {
                ImGui::PushID(layer.second);

                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
                _bool open = ImGui::TreeNodeEx("Layer", flags, "Layer: %s", layer.first.c_str());

                if (ImGui::IsItemClicked()) {
                    state.pLayer = layer.second;
                    state.pObject = nullptr;
                }

                if (open) {
                    for (auto* object : layer.second->GetObjects()) {
                        if (!object) continue;
                        if (filter.IsActive() && !filter.PassFilter(object->GetInstanceTag().c_str()))
                            continue;

                        ImGui::PushID(object);
                        _bool selected = (state.pObject == object);
                        if (ImGui::Selectable(object->GetInstanceTag().c_str(), selected)) {
                            state.pObject = object;
                            state.pLayer = layer.second;
                        }
                        ImGui::PopID();
                    }
                    ImGui::TreePop();
                }   

                ImGui::PopID();
            }
        }
    }
    ImGui::End();
}

void OutlinerPanel::Free()
{
	__super::Free();
}

#endif