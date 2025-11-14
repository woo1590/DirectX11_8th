#include "EnginePCH.h"
#include "RenderTargetPanel.h"
#include "RenderTargetManager.h"
#include "RenderTarget.h"

#ifdef USE_IMGUI

RenderTargetPanel::RenderTargetPanel()
	:IPanel()
{
}

RenderTargetPanel* RenderTargetPanel::Create()
{
	RenderTargetPanel* Instance = new RenderTargetPanel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT RenderTargetPanel::Initialize()
{
	return S_OK;
}

void RenderTargetPanel::Draw(GUIState& state)
{
    static _bool show = true;

    if (ImGui::Begin("RenderTargets", &show, ImGuiWindowFlags_MenuBar)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::TextUnformatted("RenderTargets");
            ImGui::EndMenuBar();
        }

        auto& renderTargets = state.pRenderTargetManager->GetRenderTargets();

        for (const auto& target : renderTargets) {
            ImGui::PushID(target.second);

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
            _bool open = ImGui::TreeNodeEx("Target", flags, "Target Name: %s", target.first.c_str());

            if (open) 
            {
                ID3D11ShaderResourceView* srv = target.second->GetSRV();
                ImGui::Image((ImTextureID)srv, ImVec2(500.f, 500.f));

                ImGui::TreePop();
            }

            ImGui::PopID();
        }
    }
    ImGui::End();
}

void RenderTargetPanel::Free()
{
	__super::Free();
}


#endif