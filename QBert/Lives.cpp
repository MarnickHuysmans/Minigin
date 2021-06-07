#include "Lives.h"
#include "GameObject.h"
#include "TextComponent.h"

qbert::Lives::Lives(int lives) :
	m_Lives(lives)
{
}

void qbert::Lives::Start()
{
	if (m_GameObject.expired())
	{
		return;
	}
	m_TextComponent = m_GameObject.lock()->GetComponent<dae::TextComponent>();
	SetText();
}

void qbert::Lives::QbertLives(int lives)
{
	m_Lives = lives;
	SetText();
}

void qbert::Lives::QbertRespawn()
{
}

void qbert::Lives::SetText()
{
	if (m_TextComponent.expired())
	{
		return;
	}
	m_TextComponent.lock()->SetText(std::to_string(m_Lives));
}
