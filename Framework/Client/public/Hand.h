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

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    class HandIdle : public State
    {
        void Enter(Object* object) override {};
        void Update(Object* object, _float dt) override {};
        void TestForExit(Object* object) override {};
    };

    HandIdle m_HandIdle;
};

NS_END