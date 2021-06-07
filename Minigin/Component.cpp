#include "MiniginPCH.h"
#include "Component.h"
#include "GameObject.h"

void dae::Component::RootUpdate()
{
	if (!m_Active)
	{
		return;
	}
	Update();
}

std::weak_ptr<dae::GameObject> dae::Component::GetGameObject() const
{
	return m_GameObject;
}

bool dae::Component::ActiveInScene() const
{
	if (!m_Active || m_GameObject.expired())
	{
		return false;
	}
	return m_GameObject.lock()->ActiveInScene();
}

dae::Component::~Component()
{
}
