#pragma once
#include "Level.h"

NS_BEGIN(Importer)

class ImportLevel :
    public Level
{
private:
    ImportLevel();
    virtual ~ImportLevel() = default;

public:
    static ImportLevel* Create();
    HRESULT Initialize();

    void Update(_float dt)override;
    HRESULT Render()override;

private:
};

NS_END