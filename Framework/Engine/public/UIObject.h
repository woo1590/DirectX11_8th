#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL UIObject abstract:
    public Object
{
public:
    struct UIObjectDesc : public Object::ObjectDESC
    {
        _float x,y,sizeX,sizeY;
    };

protected:
    UIObject();
    UIObject(const UIObject& prototype);
    virtual ~UIObject() = default;

public:
    virtual HRESULT Initialize_Prototype()override;
    virtual HRESULT Initialize(InitDESC* arg)override;
    virtual void PriorityUpdate(_float dt)override;
    virtual void Update(_float dt)override;
    virtual void LateUpdate(_float dt)override;
    virtual HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    virtual void Free()override;

protected:
    void UpdateTransform();

    _float m_fX = 0.f;
    _float m_fY = 0.f;
    _float m_fSizeX = 1.f;
    _float m_fSizeY = 1.f;

    _float m_fViewportWidth{};
    _float m_fViewportHeight{};

    _bool m_isDirty = true;
};

NS_END