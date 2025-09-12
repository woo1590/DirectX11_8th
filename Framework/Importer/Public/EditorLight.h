#pragma once
#include "Object.h"

NS_BEGIN(Importer)

class EditorLight :
    public Object
{
private:
    EditorLight();
    EditorLight(const EditorLight& prototype);
    virtual ~EditorLight() = default;

public:
    static EditorLight* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    
    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END