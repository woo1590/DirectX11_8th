#pragma once
#include "Object.h"

NS_BEGIN(Client)

class StageFont :
    public Object
{
public:
    typedef struct tagStageFontDesc : public Object::OBJECT_DESC
    {
        LevelID stageID{};
    }STAGE_FONT_DESC;
private:
    StageFont();
    StageFont(const StageFont& prototype);
    virtual ~StageFont() = default;

public:
    static StageFont* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT DrawFont();
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    LevelID m_eLevelID{};
    _wstring m_strStageText{};

    std::vector<_float2> m_Offsets;
    class StageFontIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime{};
        _float m_fDuration = 3.f;
    };
    class StageFontDisappear : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime{};
        _float m_fDuration = 1.f;
    };

    StageFontIdle m_StageFontIdle;
    StageFontDisappear m_StageFontDisappear;
};

NS_END