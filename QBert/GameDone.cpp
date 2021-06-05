#include "GameDone.h"
#include "GameObject.h"
#include "UIComponent.h"

void qbert::GameDone::Start()
{
	auto uiComponents = m_GameObject->GetUIComponents();
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
