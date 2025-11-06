#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Chest :
    public Object
{
public:
    typedef struct tagChestDesc : public Object::OBJECT_DESC
    {
        WeaponID weaponID = WeaponID::Count;
    }CHEST_DESC;
private:
    enum class AnimationState
    {
        Closed,
        Open,
        Close,
        Opened,
        Count
    };
    Chest();
    Chest(const Chest& prototype);
    virtual ~Chest() = default;

public:
    static Chest* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* otherCollider)override;
    void OnCollisionStay(ColliderComponent* otherCollider)override;
    void OnCollisionExit(ColliderComponent* otherCollider)override {};

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    WeaponID m_eSpawnWeaponID = WeaponID::Count;

    class ChestClosed : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class ChestOpen : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _bool m_IsWeaponSpawned = false;
    };
    class ChestClose : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class ChestOpened : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };

    ChestClosed m_ChestClosed;
    ChestOpen m_ChestOpen;
    ChestClose m_ChestClose;
    ChestOpened m_ChestOpend;
};

NS_END