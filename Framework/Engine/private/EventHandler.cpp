#include "EnginePCH.h"
#include "EventHandler.h"
#include "EngineCore.h"

EventHandler::~EventHandler()
{
	EngineCore::GetInstance()->UnSubscribe(this);
}
