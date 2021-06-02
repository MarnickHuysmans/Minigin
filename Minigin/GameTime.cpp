#include "MiniginPCH.h"
#include "GameTime.h"

#include <thread>

using namespace dae;

void GameTime::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	const auto deltaTime = currentTime - m_LastTime;
	m_LastTime = currentTime;
	const auto sleepTime = m_FrameTime - deltaTime;
	if (sleepTime.count() > 0)
	{
		m_DeltaTime = m_FrameTime.count();
		std::this_thread::sleep_for(sleepTime);
	}
	else
	{
		m_DeltaTime = std::chrono::duration<float>(deltaTime).count();
	}
}

float GameTime::GetDeltaTime() const
{
	return m_DeltaTime;
}

GameTime::GameTime()
{
	m_FrameTime = std::chrono::duration<double>(1.0 / static_cast<double>(m_DesiredFPS));
}
