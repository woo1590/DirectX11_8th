#pragma once
#include "pch.h"

NS_BEGIN(Client)

class IApp
{
protected:
	IApp() {};
	virtual ~IApp() = default;

public:
	virtual HRESULT Initialize() = 0;
	virtual void Update(_float dt) = 0;

};

NS_END