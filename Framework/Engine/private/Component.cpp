#include "EnginePCH.h"
#include "Component.h"

Component::Component(Object* owner)
	:m_pOwner(owner)
{
}

Component::Component(const Component& prototype)
	:m_pOwner(nullptr)
{

}
