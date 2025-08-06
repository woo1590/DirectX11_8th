#include "EnginePCH.h"
#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh* Mesh::Create()
{
	return nullptr;
}

HRESULT Mesh::Initialize()
{
	return E_NOTIMPL;
}

void Mesh::Free()
{
	__super::Free();
}
