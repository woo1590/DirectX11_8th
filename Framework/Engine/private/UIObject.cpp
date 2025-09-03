#include "EnginePCH.h"
#include "UIObject.h"

UIObject::UIObject()
	:Object()
{
}

UIObject::UIObject(const UIObject& prototype)
	:Object(prototype)
{
}

void UIObject::Free()
{
	__super::Free();
}
