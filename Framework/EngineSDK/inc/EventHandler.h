#pragma once
#include "EventSystem.h"
NS_BEGIN(Engine)

class ENGINE_DLL EventHandler
{
protected:
	EventHandler() {};
	virtual ~EventHandler() = default;

public:
	template <typename T>
	LISTENER MakeListener(void (T::* func)(std::any))
	{
		static_assert(std::is_base_of_v<EventHandler, T>, "T must be derived from EventHandler");
		return LISTENER(this, [this, func](std::any param) { (static_cast<T*>(this)->*func)(param); });
	}
};

NS_END