#include "pch.h"
#include "EffectEditPanel.h"
#include "EngineCore.h"

EffectEditPanel::EffectEditPanel()
	:IPanel()
{
}

EffectEditPanel* EffectEditPanel::Create()
{
	EffectEditPanel* Instance = new EffectEditPanel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EffectEditPanel::Initialize()
{
	auto engine = EngineCore::GetInstance();

	return S_OK;
}

void EffectEditPanel::Draw(GUIState& state)
{

}

void EffectEditPanel::Free()
{
	__super::Free();
}
