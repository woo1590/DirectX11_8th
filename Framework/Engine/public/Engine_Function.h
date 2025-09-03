#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

namespace Engine
{
	template<typename T>
	void Safe_Delete(T& p)
	{
		if (p)
		{
			delete p;
			p = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& p)
	{
		if (p)
		{
			delete[] p;
			p = nullptr;
		}
	}

	template<typename T>
	_ulong Safe_Release(T& instance)
	{
		_ulong dwRefCnt = 0;

		if (instance)
		{
			dwRefCnt = instance->Release();

			if (!dwRefCnt)
				instance = nullptr;
		}

		return dwRefCnt;
	}
}

#endif // Engine_Function_h__
