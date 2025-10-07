#pragma once
#include "Object.h"

NS_BEGIN(Importer)

class EditorGrid :
    public Object
{
private:
    EditorGrid();
    EditorGrid(const EditorGrid& prototype);
    virtual ~EditorGrid() = default;

public:
    static EditorGrid* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END