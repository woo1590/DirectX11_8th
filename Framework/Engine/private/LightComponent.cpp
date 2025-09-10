#include "EnginePCH.h"
#include "LightComponent.h"
#include "EngineCore.h"
#include "Object.h"
#include "TransformComponent.h"

LightComponent::LightComponent(Object* owner)
	:Component(owner)
{
}

LightComponent::LightComponent(const LightComponent& prototype)
	:Component(prototype)
{
}

LightComponent* LightComponent::Create(Object* owner)
{
	LightComponent* Instance = new LightComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LightComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT LightComponent::Initialize(InitDESC* arg)
{
	if (arg)
	{
		LIGHT_DESC* desc = static_cast<LIGHT_DESC*>(arg);
		
		m_eType = desc->type;
		m_Color = desc->color;
		m_Direction = desc->direction;
		m_fRange = desc->range;

		EngineCore::GetInstance()->RegisterLight(this);
	}
	else
	{
		MSG_BOX("Failed to Initalize : LightComponent");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT LightComponent::ExtractLightProxy(std::vector<LightProxy>& lights)
{
	LightProxy proxy;
	_float3 position = m_pOwner->GetComponent<TransformComponent>()->GetPosition();

	proxy.type = m_eType;
	proxy.lightColor = m_Color;
	proxy.lightDirection = m_Direction;
	proxy.lightPosition = _float4(position.x, position.y, position.z, 1.f);
	proxy.lightRange = m_fRange;

	lights.push_back(proxy);

	return S_OK;
}

void LightComponent::Free()
{
	__super::Free();

	EngineCore::GetInstance()->UnRegisterLight(this);
}

#ifdef USE_IMGUI
void LightComponent::RenderInspector()
{
	ImGui::PushID(this);

	if (!ImGui::CollapsingHeader("Light",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		ImGui::Separator();

		ImGui::DragFloat4("Color", &m_Color.x, 0.1f, 0.f, 1.f);
		ImGui::DragFloat4("Direction", &m_Direction.x, 0.1f, -FLT_MAX, FLT_MAX);
	}

	ImGui::PopID();

}
#endif