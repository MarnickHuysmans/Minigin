#pragma once
#include <memory>

#include "Level.h"
#include "LevelEnums.h"

namespace dae {
	class Scene;
}

namespace qbert
{
	class LevelFactory
	{
	public:
		static const std::weak_ptr<Level> CreateLevel(dae::Scene& scene, LevelType type, int discAmount = 2, int scale = 1, int levelSize = 7);

	private:
		static int m_CubeHeightOffset;
	};
}
