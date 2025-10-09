#include "MapEditorPCH.h"
#include "NavPickable.h"

//object
#include "Object.h"

//component
#include "NavDataComponent.h"

NavPickable::NavPickable(Object* owner)
	:PickableComponent(owner)
{
}

NavPickable::NavPickable(const NavPickable& prototype)
	:PickableComponent(prototype)
{
}

NavPickable * NavPickable::Create(Object* owner)
{
	NavPickable* Instance = new NavPickable(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT NavPickable::Initialize_Prototype()
{
	return S_OK;
}

HRESULT NavPickable::Initialize(InitDESC* arg)
{
	return S_OK;
}

PICK_RESULT NavPickable::IntersectRay(RAY ray)
{
	PICK_RESULT result{};
	auto& navCellDatas = m_pOwner->GetComponent<NavDataComponent>()->GetNavCellDatas();
	
	for (const auto& cellData : navCellDatas)
	{
		
	}


	return result;
}

void NavPickable::Free()
{
	__super::Free();
}
