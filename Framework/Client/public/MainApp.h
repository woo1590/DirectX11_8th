#pragma once

#include "Base.h"
#include "IApp.h"

NS_BEGIN(Client)

class MainApp final :
    public Base, public IApp
{
private:
    MainApp();
    virtual ~MainApp() = default;

public:
    static MainApp* Create();
    HRESULT Initialize()override;
    void Update(_float dt)override;
    void Free()override;

private:

};

NS_END