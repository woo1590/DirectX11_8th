#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class PlayerCam :
    public PartObject
{
public:
    typedef struct tagPlayerCamDesc :public PartObject::PART_OBJECT_DESC
    {
    }PLAYER_CAM_DESC;
private:
    PlayerCam();
    PlayerCam(const PlayerCam& prototype);
    virtual ~PlayerCam() = default;

public:
    static PlayerCam* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END