#include "EnginePCH.h"
#include "LightManager.h"
#include "LightComponent.h"

LightManager::LightManager()
{
}

LightManager* LightManager::Create()
{
	LightManager* Instance = new LightManager();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LightManager::Initialize()
{
	return S_OK;
}

void LightManager::RegisterLight(LightComponent* light)
{
	m_Lights.push_back(light);
}

void LightManager::UnRegisterLight(LightComponent* light)
{
	auto iter = std::find(m_Lights.begin(), m_Lights.end(), light);

	if (iter != m_Lights.end())
		m_Lights.erase(iter);
}

HRESULT LightManager::ExtractLightProxy(std::vector<LightProxy>& lights)
{
	for (const auto& light : m_Lights)
	{
		if (FAILED(light->ExtractLightProxy(lights)))
			return E_FAIL;
	}

	return S_OK;
}

void LightManager::Free()
{
	__super::Free();

	for (auto& light : m_Lights)
		Safe_Release(light);

	m_Lights.clear();
}
