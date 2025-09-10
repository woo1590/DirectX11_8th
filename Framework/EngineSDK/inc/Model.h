#pragma once
#include "Base.h"
#include "RenderProxy.h"

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

    HRESULT ExtractRenderProxy(RenderProxy& proxy);

    void SetRootNode(Node rootNode);
    void Free()override;

private:
    HRESULT ProcessNodeForRenderProxy();    //노드 재귀 순회

    Node* m_pRootNode = nullptr;

};

NS_END