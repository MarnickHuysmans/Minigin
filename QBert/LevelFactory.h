#pragma once
#include <memory>

#include "Level.h"

namespace dae {
	class Scene;
}

namespace qbert
{
	enum class LevelType;

	class LevelFactory
	{
	public:
		static const std::weak_ptr<Level> CreateLevel(dae::Scene& scene, LevelType type, int discAmount = 2, float scale = 1.0f, int levelSize = 7);

	private:
		static int m_CubeHeightOffset;
	};
}
