#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Cell;
class ENGINE_DLL NavMesh :
    public Base
{
private:
    NavMesh();
    virtual ~NavMesh() = default;

public:
    static NavMesh* Create(const _string& filePath);
    HRESULT Initialize(const _string& filePath);

    void Free()override;

private:
    _uint m_iNumCells{};
    std::vector<Cell*> m_Cells;
};

NS_END