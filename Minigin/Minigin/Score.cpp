#include "MiniginPCH.h"
#include "Score.h"
#include "GameObject.h"
#include "TextComponent.h"

void qbert::Score::Start()
{
	auto textComponent = m_GameObject->GetComponent<TextComponent>();
	if (!textComponent.expired())
	{
		m_TextComponent = textComponent.lock().get();
		m_TextComponent->SetText("0");
	}
}

void qbert::Score::Update()
{
}

void qbert::Score::Notify(const std::string& message)
{
	if (message == "Color")
	{
		m_Score += 25;
	}
	else if (message == "Coily") {
		m_Score += 500;
	}
	else if (message == "Discs")
	{
		m_Score += 50;
	}
	else if (message == "SlickSam")
	{
		m_Score += 300;
	}
	m_TextComponent->SetText(std::to_string(m_Score));
}
