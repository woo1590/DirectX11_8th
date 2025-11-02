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
	result.type = PickType::Nav;

	auto navData = m_pOwner->GetComponent<NavDataComponent>();
	auto& navCellDatas = navData->GetNavCellDatas();
	auto& navMaterials = navData->GetMaterialInstances();

	_float minDistance = FLT_MAX;
	for (_uint i=0; i<navCellDatas.size(); ++i)
	{
		_float distance = FLT_MAX;
		NAVCELL_DATA cellData = navCellDatas[i];
		_vector p0 = XMLoadFloat3(&cellData.points[0]);
		_vector p1 = XMLoadFloat3(&cellData.points[1]);
		_vector p2 = XMLoadFloat3(&cellData.points[2]);

		if (TriangleTests::Intersects(XMLoadFloat3(&ray.origin), XMLoadFloat3(&ray.direction), p0, p1, p2, distance))
		{
			if (distance < minDistance)
			{
				minDistance = distance;

				result.isHit = true;
				result.distance = minDistance;
				result.navCellIndex = cellData.index;
			}
		}
	}

	if (result.isHit)
	{
		navData->SetHoverIndex(result.navCellIndex);
		XMStoreFloat3(&result.worldHitPosition, XMLoadFloat3(&ray.origin) + result.distance * XMLoadFloat3(&ray.direction));
	}
	else
		navData->SetHoverIndex(-1);

	return result;
}

void NavPickable::Free()
{
	__super::Free();
}
