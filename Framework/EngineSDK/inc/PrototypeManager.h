#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class PrototypeManager final:
    public Base
{
private:
    PrototypeManager();
    virtual ~PrototypeManager() = default;

public:
    static PrototypeManager* Create(_uint levelCnt);
    HRESULT Initialize(_uint levelCnt);
    void Free()override;

    HRESULT AddPrototype(_uint level, const _string& prototypeTag, Base* prototype);
    Base * ClonePrototype(Prototype type, _uint level, const _string& prototypeTag, InitDESC* arg);
    void Clear(_uint level);

private:
    Base* FindPrototype(_uint level, const _string & prototypeTag);

    _uint m_iLevelCnt = 0;
    std::vector<std::unordered_map<_string, Base*>> m_Prototypes;
};

NS_END