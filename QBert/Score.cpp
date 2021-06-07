#include "Score.h"
#include "GameObject.h"
#include "TextComponent.h"

qbert::Score::Score() :
	m_Score(0)
{
}

void qbert::Score::Start()
{
	if (m_GameObject.expired())
	{
		return;
	}
	m_TextComponent = m_GameObject.lock()->GetComponent<dae::TextComponent>();
}

void qbert::Score::AddScore(unsigned score)
{
	m_Score += score;
	if (m_TextComponent.expired())
	{
		return;
	}
	m_TextComponent.lock()->SetText(std::to_string(m_Score));
}
