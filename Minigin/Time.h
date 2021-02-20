#pragma once
#include <chrono>

#include "Singleton.h"

namespace dae
{
	class Time final : public Singleton<Time>
	{
	public:
		void Update();
		
		float GetDeltaTime() const;

	private:
		friend class Singleton<Time>;
		Time();
		float m_DesiredFPS{ 60.0f };
		std::chrono::duration<double> m_FrameTime{};
		std::chrono::steady_clock::time_point m_LastTime{ std::chrono::high_resolution_clock::now() };
		float m_DeltaTime{};
	};
}

