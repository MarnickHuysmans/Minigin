#include "GameOver.h"
#include "GameObject.h"
#include "UIComponent.h"

void qbert::GameOver::Start()
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

void qbert::GameOver::QbertLives(int lives)
{
	if (lives != 0 || m_UiComponent.expired())
	{
		return;
	}
	m_UiComponent.lock()->SetActive(true);
}

void qbert::GameOver::QbertRespawn()
{
}
