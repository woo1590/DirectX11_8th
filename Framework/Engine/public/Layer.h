#pragma once
#include "Base.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Object;
class ENGINE_DLL Layer final:
    public Base
{
private:
    Layer();
    virtual ~Layer() = default;

public:
    static Layer* Create();
    HRESULT Initialize();

    void PriorityUpdate(_float dt);
    void Update(_float dt);
    void LateUpdate(_float dt);

    void AddObject(Object* object);

    void SetUpdatable(_bool update) { m_isUpdatable = update; }
    _bool IsUpdatable()const { return m_isUpdatable; }
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);
    std::list<Object*>& GetObjects() { return m_Objects; }

    Object* GetFrontObject()const;
    Object* GetLastObject()const;
    Object* GetObjectByInstanceTag(const _string& instanceTag);

    void Free()override;

private:
    _bool m_isUpdatable = true;
    std::list<Object*> m_Objects;

};

NS_END