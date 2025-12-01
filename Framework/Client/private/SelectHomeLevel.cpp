#include "pch.h"
#include "SelectHomeLevel.h"
#include "EngineCore.h"

SelectHomeLevel::SelectHomeLevel()
	:Level()
{
}

SelectHomeLevel* SelectHomeLevel::Create()
{
	SelectHomeLevel* Instance = new SelectHomeLevel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT SelectHomeLevel::Initialize()
{

}

void SelectHomeLevel::Free()
{
}

void SelectHomeLevel::Update(_float dt)
{
}

HRESULT SelectHomeLevel::Render()
{
	return E_NOTIMPL;
}

HRESULT SelectHomeLevel::Initialize_LayerHome(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_SelectHome", ENUM_CLASS(LevelID::SelectHome), layerTag)))
		return E_FAIL;
}

HRESULT SelectHomeLevel::Initialize_LayerCharacter(const _string& layerTag)
{
	return E_NOTIMPL;
}
