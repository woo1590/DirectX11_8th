#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class SkillPanel :
    public UIObject
{
public:
    typedef struct tagSkillPanelDesc : public UIObject::UIOBJECT_DESC
    {
        _uint numPanel{};
        SkillID skillID = SkillID::Count;

    }SKILL_PANEL_DESC;

private:
    enum class Parts { Slot, Count };

private:
    SkillPanel();
    SkillPanel(const SkillPanel& prototype);
    virtual ~SkillPanel() = default;

public:
    static SkillPanel* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    /*Event*/
    void OnJump(std::any param);
    void OnLand(std::any param);
    void OnDash(std::any param);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreateChildren();

    class SkillPanelIdle : public State
    {
        void Enter(Object* object)override{}
        void Update(Object* object, _float dt)override{}
        void TestForExit(Object* object)override{}
    };
    class SkillPanelOnJump : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDuration = 0.5f;
        _float m_fElapsedTime = 0.f;
        _float3 m_StartPosition{};
        _float3 m_TargetPosition{};
    };
    class SkillPanelOnLand : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDuration = 0.3f;
        _float m_fElapsedTime = 0.f;
        _float3 m_StartPosition{};
        _float3 m_TargetPosition{};
    };
    class SkillPanelOnDash : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDuration = 0.5f;
        _float m_fElapsedTime = 0.f;
        _float3 m_StartPosition{};
        _float3 m_TargetPosition{};
    };

    SkillPanelIdle m_SkillPanelIdle;
    SkillPanelOnJump m_SkillPanelOnJump;
    SkillPanelOnLand m_SkillPanelOnLand;
    SkillPanelOnDash m_SkillPanelOnDash;

};

NS_END