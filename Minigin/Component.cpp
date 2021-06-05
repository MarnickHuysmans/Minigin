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

std::shared_ptr<dae::GameObject> dae::Component::GetGameObject() const
{
	return m_GameObject->shared_from_this();
}

bool dae::Component::ActiveInScene() const
{
	if (!m_Active || m_GameObject == nullptr)
	{
		return false;
	}
	return m_GameObject->ActiveInScene();
}

dae::Component::~Component()
{
}
