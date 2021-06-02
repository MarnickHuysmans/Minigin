#pragma once
#include <chrono>

#include "Singleton.h"

namespace dae
{
	class GameTime final : public Singleton<GameTime>
	{
	public:
		void Update();
		
		float GetDeltaTime() const;

	private:
		friend class Singleton<GameTime>;
		GameTime();
		float m_DesiredFPS{ 60.0f };
		std::chrono::duration<float> m_FrameTime{};
		std::chrono::steady_clock::time_point m_LastTime{ std::chrono::high_resolution_clock::now() };
		float m_DeltaTime{};
	};
}

