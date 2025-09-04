#include "EnginePCH.h"
#include "OutlinerPanel.h"
#include "ObjectManager.h"
#include "Object.h"
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

    if (ImGui::Begin("Outliner", &show, ImGuiWindowFlags_MenuBar)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::TextUnformatted("Outliner");
            ImGui::EndMenuBar();
        }

        static ImGuiTextFilter filter;
        filter.Draw("Filter", 180.0f);

        // 예: 레벨 ID 3의 레이어들
        const auto& layers = state.pObjectManager->GetLayers(3);

        for (const auto& layer : layers) {
            // layer.first = 이름, layer.second = Layer*
            if (!filter.IsActive() || filter.PassFilter(layer.first.c_str())) {
                ImGui::PushID(layer.second); // ★ ID 충돌 방지

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

                        ImGui::PushID(object); // ★ 오브젝트 단위로도 범위
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