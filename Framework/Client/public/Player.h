#pragma once
#include "ContainerObject.h"

NS_BEGIN(Client)

class Player final:
    public ContainerObject
{
public:
    enum class Parts { Hand, PlayerCam, Count };
private:
    Player();
    Player(const Player& prototype);
    virtual ~Player() = default;

public:
    static Player* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreatePartObjects();
    void KeyInput(_float dt);
};

NS_END