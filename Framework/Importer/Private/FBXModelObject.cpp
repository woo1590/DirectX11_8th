#include "ImporterPCH.h"
#include "FBXModelObject.h"

//component
#include "FBXLoaderComponent.h"

FBXModelObject::FBXModelObject()
	:Object()
{
}

FBXModelObject::FBXModelObject(const FBXModelObject& prototype)
	:Object(prototype)
{
}

FBXModelObject* FBXModelObject::Create()
{
	FBXModelObject* Instance = new FBXModelObject();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FBXModelObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "FBXModelObject";

	AddComponent<FBXLoaderComponent>();

	return S_OK;
}

HRESULT FBXModelObject::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;
	
	auto loader = GetComponent<FBXLoaderComponent>();
	loader->Initialize(nullptr);

	return S_OK;
}

void FBXModelObject::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void FBXModelObject::Update(_float dt)
{
	__super::Update(dt);
}

void FBXModelObject::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT FBXModelObject::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	auto modelLoader = GetComponent<FBXLoaderComponent>();
	
	modelLoader->ExtractRenderProxies(m_pTransform, proxies[ENUM_CLASS(RenderGroup::NonBlend)]);

	return S_OK;
}

Object* FBXModelObject::Clone(InitDESC* arg)
{
	FBXModelObject* Instance = new FBXModelObject(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void FBXModelObject::Free()
{
	__super::Free();
}
