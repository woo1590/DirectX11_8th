#include "MapEditorPCH.h"
#include "Door_R.h"

//component
#include "ModelComponent.h"

Door_R::Door_R()
	:PartObject()
{
}

Door_R::Door_R(const Door_R& prototype)
	:PartObject(prototype)
{
}

Door_R* Door_R::Create()
{
	Door_R* Instance = new Door_R();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Door_R::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	m_strInstanceTag = "Door_R";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Door_R::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Editor), "Model_Door_R");

	return S_OK;
}

Object* Door_R::Clone(InitDESC* arg)
{
	Door_R* Instance = new Door_R(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Door_R::Free()
{
	__super::Free();
}
