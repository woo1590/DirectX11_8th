#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Layer;
class ObjectManager :
    public Base
{
private:
    ObjectManager();
    virtual ~ObjectManager() = default;

public:
    static ObjectManager* Create(_uint levelCnt);
    HRESULT Initialize(_uint levelCnt);
    void Free()override;

    void Update(_float dt);
    void LateUpdate(_float dt);

    HRESULT AddObject(_uint prototypeLevelID, const _string& prototypeTag, _uint layerLevelID, const _string& layerTag, void* arg = nullptr);
    void Clear(_uint levelID);

private:
    Layer* FindLayer(_uint layerLevel, const _string& layerTag);

    std::vector<std::unordered_map<std::string, Layer*>> layers;

};

NS_END