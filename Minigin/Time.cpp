#include "MiniginPCH.h"
#include "Time.h"

#include <thread>

using namespace dae;

void Time::Update()
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

float Time::GetDeltaTime() const
{
	return m_DeltaTime;
}

Time::Time()
{
	m_FrameTime = std::chrono::duration<double>(1.0 / static_cast<double>(m_DesiredFPS));
}
