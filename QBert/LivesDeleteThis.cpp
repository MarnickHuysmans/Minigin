#include "LivesDeleteThis.h"

#include "GameObject.h"
#include "QbertDeleteThis.h"
#include "TextComponent.h"

void qbert::LivesDeleteThis::Start()
{
	if (m_GameObject.expired())
	{
		return;
	}
	auto textComponent = m_GameObject.lock()->GetComponent<dae::TextComponent>();
	if (!textComponent.expired())
	{
		m_TextComponent = textComponent.lock().get();
		m_TextComponent->SetText(std::to_string(m_Qbert->GetLives()));
	}
}

void qbert::LivesDeleteThis::Update()
{
}

void qbert::LivesDeleteThis::Notify(const std::string& message)
{
	if (m_TextComponent && message == "PlayerDied")
	{
		m_TextComponent->SetText(std::to_string(m_Qbert->GetLives()));
	}
}

qbert::LivesDeleteThis::LivesDeleteThis(QbertDeleteThis* qbert)
	:m_Qbert(qbert)
{
}