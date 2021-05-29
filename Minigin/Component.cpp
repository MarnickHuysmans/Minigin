#include "MiniginPCH.h"
#include "Component.h"

const dae::GameObject& dae::Component::GetGameObject() const
{
	return *m_GameObject;
}

dae::Component::~Component()
{
}
