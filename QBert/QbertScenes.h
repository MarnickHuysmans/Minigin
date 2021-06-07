#pragma once
#include "Scene.h"

namespace qbert
{
	class StartScene
	{
	public:
		void operator()(dae::Scene& scene) const;
	};

	class GameScene
	{
	public:
		enum class GameType
		{
			Single,
			Coop,
			Versus
		};

		void operator()(dae::Scene& scene) const;
		static void SetGameType(GameType gameType);

	private:
		static GameType m_GameType;
	};
}
