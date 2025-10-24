#include "MapEditorPCH.h"
#include "Door_L.h"

//component
#include "ModelComponent.h"

Door_L::Door_L()
	:PartObject()
{
}

Door_L::Door_L(const Door_L& prototype)
	:PartObject(prototype)
{
}

Door_L* Door_L::Create()
{
	Door_L* Instance = new Door_L();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Door_L::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	m_strInstanceTag = "Door_L";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Door_L::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Editor), "Model_Door_L");

	return S_OK;
}

Object* Door_L::Clone(InitDESC* arg)
{
	Door_L* Instance = new Door_L(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Door_L::Free()
{
	__super::Free();
}
