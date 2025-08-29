#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Material;
class VIBuffer;

struct Node
{
    std::pair<VIBuffer*, Material*> bufferMaterial;
    std::vector<Node*> children;
};

class ENGINE_DLL Model :
    public Base
{
private:
    Model();
    virtual ~Model() = default;

public:
    static Model* Create();
    HRESULT Initialize();

    void Free()override;

private:
    Node* m_pRootNode = nullptr;

};

NS_END