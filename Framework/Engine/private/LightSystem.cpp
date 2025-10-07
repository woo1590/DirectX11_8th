#include "EnginePCH.h"
#include "LightSystem.h"
#include "LightComponent.h"

LightSystem::LightSystem()
{
}

LightSystem* LightSystem::Create()
{
	LightSystem* Instance = new LightSystem();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LightSystem::Initialize()
{
	return S_OK;
}

void LightSystem::RegisterLight(LightComponent* light)
{
	m_Lights.push_back(light);
}

void LightSystem::UnRegisterLight(LightComponent* light)
{
	auto iter = std::find(m_Lights.begin(), m_Lights.end(), light);

	if (iter != m_Lights.end())
		m_Lights.erase(iter);
}

HRESULT LightSystem::ExtractLightProxy(std::vector<LightProxy>& lights)
{
	for (const auto& light : m_Lights)
	{
		if (FAILED(light->ExtractLightProxy(lights)))
			return E_FAIL;
	}

	return S_OK;
}

void LightSystem::Free()
{
	__super::Free();

	for (auto& light : m_Lights)
		Safe_Release(light);

	m_Lights.clear();
}
