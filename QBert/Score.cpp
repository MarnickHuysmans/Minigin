#include "Score.h"
#include "GameObject.h"
#include "TextComponent.h"

qbert::Score::Score() :
	m_Score(0)
{
}

void qbert::Score::Start()
{
	m_GameObject->GetComponent<dae::TextComponent>();
}

void qbert::Score::AddScore(unsigned score)
{
	if (m_TextComponent.expired())
	{
		return;
	}
	m_TextComponent.lock()->SetText(std::to_string(m_Score));
}
