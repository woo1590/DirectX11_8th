#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Fracture :
    public Object
{
public:
    typedef struct tagFractureDesc : public Object::OBJECT_DESC
    {
        _uint spawnNavCell{};
        _string modelTag{};
    }FRACTURE_DESC;
private:
    Fracture();
    Fracture(const Fracture& prototype);
    virtual ~Fracture() = default;

public:
    static Fracture* Create();
    virtual HRESULT Initialize_Prototype()override;
    virtual HRESULT Initialize(InitDESC* arg)override;
    virtual void PriorityUpdate(_float dt)override;
    virtual void Update(_float dt)override;
    virtual void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fLifeDuration = 2.5f;
    _float m_fElapsedTime = 0.f;
};

NS_END