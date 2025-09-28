#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Hand :
    public PartObject
{
public:
    typedef struct HandDesc : public PartObject::PART_OBJECT_DESC
    {
     
    }HAND_DESC;
private:
    Hand();
    Hand(const Hand& prototype);
    virtual ~Hand() = default;

public:
    static Hand* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END