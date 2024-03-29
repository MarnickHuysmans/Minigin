#include "MiniginPCH.h"
#include "FPS.h"
#include <iomanip>
#include "GameObject.h"
#include "TextComponent.h"
#include "GameTime.h"

void dae::FPS::Start()
{
	if (m_GameObject.expired())
	{
		return;
	}
	auto textComponent = m_GameObject.lock()->GetComponent<TextComponent>();
	if (!textComponent.expired())
	{
		m_TextComponent = textComponent.lock().get();
	}
}

void dae::FPS::Update()
{
	if (m_TextComponent)
	{
		float deltaTime = GameTime::GetInstance().GetDeltaTime();
		m_Counter += deltaTime;
		m_DeltaTime += deltaTime;
		++m_DeltaCount;
		if (m_Counter < 1.0f)
		{
			return;
		}
		float fps{};
		if (m_DeltaTime != 0)
		{
			m_DeltaTime /= static_cast<float>(m_DeltaCount);
			fps = 1.0f / deltaTime;
		}
		std::stringstream stream{};
		stream << std::fixed << std::setprecision(1) << fps << " FPS";
		m_TextComponent->SetText(stream.str());
		m_DeltaTime = 0.0f;
		m_Counter -= 1.0f;
		m_DeltaCount = 0;
	}
}
