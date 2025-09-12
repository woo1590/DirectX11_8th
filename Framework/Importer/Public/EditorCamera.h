#pragma once
#include "Object.h"

NS_BEGIN(Importer)

class EditorCamera :
    public Object
{
private:
    EditorCamera();
    EditorCamera(const EditorCamera& prototype);
    virtual ~EditorCamera() = default;

public:
    static EditorCamera* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _bool m_ActiveMouse = true;
};
NS_END