#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class SpriteComponent;
NS_END

NS_BEGIN(Client)

class LoadingPanel :
    public UIObject
{
public:
    typedef struct tagLoadingPanelDesc : public UIObject::UIOBJECT_DESC
    {
        LevelID nextLevelID = LevelID::Count;
    }LOADING_PANEL_DESC;
private:
    LoadingPanel();
    LoadingPanel(const LoadingPanel& prototype);
    virtual ~LoadingPanel() = default;

public:
    static LoadingPanel* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector < std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    LevelID m_eNextLevelID{};

    SpriteComponent* m_pLoadingBar = nullptr;
    TransformComponent* m_pLoadingBarTransform = nullptr;

    SpriteComponent* m_pStartIcon = nullptr;
    TransformComponent* m_pStartIconTransform = nullptr;

    SpriteComponent* m_pStage2Icon = nullptr;
    TransformComponent* m_pStage2Transform = nullptr;

    SpriteComponent* m_pStage3Icon = nullptr;
    TransformComponent* m_pStage3Transform = nullptr;

    SpriteComponent* m_pBossIcon = nullptr;
    TransformComponent* m_pBossTransform = nullptr;
};

NS_END