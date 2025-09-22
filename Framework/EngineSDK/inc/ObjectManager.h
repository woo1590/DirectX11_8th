#pragma once
#include "Base.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Layer;
class Object;
class ObjectManager :
    public Base
{
private:
    ObjectManager();
    virtual ~ObjectManager() = default;

public:
    static ObjectManager* Create(_uint numLevels);
    HRESULT Initialize(_uint numLevels);
    void Free()override;

    void PriorityUpdate(_float dt);
    void Update(_float dt);
    void LateUpdate(_float dt);

    HRESULT AddObject(_uint prototypeLevelID, const _string& prototypeTag, _uint layerLevelID,
                      const _string& layerTag, InitDESC* arg = nullptr);
    Layer* GetLayer(_uint layerLevel, const _string& layerTag);
    std::unordered_map<_string, Layer*>& GetLayers(_uint levelID);
    Object* GetObjectByInstanceTag(_uint layerLevel, const _string& layerTag, const _string& instanceTag);
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    void Clear(_uint levelID);

private:
    Layer* FindLayer(_uint layerLevel, const _string& layerTag);

    std::vector<std::unordered_map<std::string, Layer*>> m_Layers;

};

NS_END