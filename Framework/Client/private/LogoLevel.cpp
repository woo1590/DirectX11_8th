#include "pch.h"
#include "EngineCore.h"
#include "LogoLevel.h"

LogoLevel::LogoLevel()
{
}

LogoLevel* LogoLevel::Create()
{
	LogoLevel* Instance = new LogoLevel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LogoLevel::Initialize()
{
	return S_OK;
}

void LogoLevel::Free()
{
	__super::Free();
}

void LogoLevel::Update(_float dt)
{
}

HRESULT LogoLevel::Render()
{
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), L"Logo Level");

	return S_OK;
}
