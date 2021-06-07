#include "GameDone.h"
#include "GameObject.h"
#include "UIComponent.h"

void qbert::GameDone::Start()
{
	if (m_GameObject.expired())
	{
		return;
	}
	auto uiComponents = m_GameObject.lock()->GetUIComponents();
	if (uiComponents.empty())
	{
		return;
	}
	m_UiComponent = uiComponents[0];
	if (m_UiComponent.expired())
	{
		return;
	}
	m_UiComponent.lock()->SetActive(false);
}

void qbert::GameDone::LevelDisc()
{
}

void qbert::GameDone::NextLevel()
{
}

void qbert::GameDone::GameComplete()
{
	if (m_UiComponent.expired())
	{
		return;
	}
	m_UiComponent.lock()->SetActive(true);
}
