#include "Lives.h"

#include "GameObject.h"
#include "QbertDeleteThis.h"
#include "TextComponent.h"

void qbert::Lives::Start()
{
	auto textComponent = m_GameObject->GetComponent<dae::TextComponent>();
	if (!textComponent.expired())
	{
		m_TextComponent = textComponent.lock().get();
		m_TextComponent->SetText(std::to_string(m_Qbert->GetLives()));
	}
}

void qbert::Lives::Update()
{
}

void qbert::Lives::Notify(const std::string& message)
{
	if (m_TextComponent && message == "PlayerDied")
	{
		m_TextComponent->SetText(std::to_string(m_Qbert->GetLives()));
	}
}

qbert::Lives::Lives(QbertDeleteThis* qbert)
	:m_Qbert(qbert)
{
}