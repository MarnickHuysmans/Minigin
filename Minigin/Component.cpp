#include "MiniginPCH.h"
#include "Component.h"
#include "GameObject.h"

std::shared_ptr<dae::GameObject> dae::Component::GetGameObject() const
{
	return m_GameObject->shared_from_this();
}

dae::Component::~Component()
{
}
