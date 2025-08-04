#include "EnginePCH.h"
#include "Base.h"

Base::Base()
{
}

unsigned int Base::AddRef()
{
	return ++refCnt;
}

unsigned int Base::Release()
{
	if (!refCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
		return refCnt--;
}
