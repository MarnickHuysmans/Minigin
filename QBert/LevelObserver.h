#pragma once

namespace qbert
{
	class LevelObserver
	{
	public:
		LevelObserver() = default;
		virtual ~LevelObserver() = default;
		virtual void NextLevel() = 0;
		virtual void GameComplete() = 0;
	};
}