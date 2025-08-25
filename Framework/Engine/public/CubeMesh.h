#pragma once
#include "Mesh.h"

NS_BEGIN(Engine)

class ENGINE_DLL CubeMesh :
    public Mesh
{
private:
    CubeMesh();
    virtual ~CubeMesh() = default;

public:
    static CubeMesh* Create();
    HRESULT Initialize()override;

    void Free()override;

private:

};

NS_END