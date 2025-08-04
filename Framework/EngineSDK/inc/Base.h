#pragma once

NS_BEGIN(Engine)

class ENGINE_DLL Base abstract
{
protected:
	Base();
	virtual ~Base() = default;

public:
	unsigned int AddRef();
	unsigned int Release();
	virtual void Free() {};

protected:
	unsigned int refCnt = 0;

};

NS_END