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

    void Open();

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
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
        void Enter(Object* object)override;
        void Update(Object* object, _float t)override;
        void TestForExit(Object* object)override;
    };
    class DoorClosing : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float t)override;
        void TestForExit(Object* object)override;
    };

    DoorOpen m_DoorOpen;
    DoorOpening m_DoorOpening;
    DoorClosed m_DoorClosed;
    DoorClosing m_DoorClosing;
};

NS_END