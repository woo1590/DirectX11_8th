#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Level abstract:
    public Base
{
protected:
    Level();
    virtual ~Level() = default;

public:
    virtual void Update(_float dt);
    virtual HRESULT Render();
    virtual void Free()override;

    const _string& GetLevelTag() { return m_strLevelTag; }
protected:
    _string m_strLevelTag{};
};

NS_END