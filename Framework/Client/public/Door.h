#pragma once
#include "ContainerObject.h"

NS_BEGIN(Client)

class Door :
    public ContainerObject
{
    enum class Parts { Door_L, Door_R, Count };
private:
    Door();
    Door(const Door& prototype);
    virtual ~Door() = default;

public:
    static Door* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void ConnectNextStage(LevelID nextLevelID);
    void Open();

    void OnCollisionStay(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _bool m_IsConnectStage = false;
    LevelID m_eConnectStageID = LevelID::Count;

    HRESULT CreatePartObjects();

    class DoorOpen : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float t)override;
        void TestForExit(Object* object)override;
    };
    class DoorOpening : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 2.5f;
    };
    class DoorClosed : public State
    {
        void Enter(Object* object)override {};
        void Update(Object* object, _float t)override {};
        void TestForExit(Object* object)override {};
    };
    class DoorConnectNextStage : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };

    DoorOpen m_DoorOpen;
    DoorOpening m_DoorOpening;
    DoorClosed m_DoorClosed;
    DoorConnectNextStage m_DoorConnectNextStage;
};

NS_END