#pragma once
#include "EnginePCH.h"

namespace Engine
{
	namespace math
	{
		inline _float ToRadian(_float degree)
		{
			return XMConvertToRadians(degree);
		}

		inline _float ToDegree(_float radian)
		{
			return XMConvertToDegrees(radian);
		}
	};
};