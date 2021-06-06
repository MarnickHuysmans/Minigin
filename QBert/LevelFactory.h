#pragma once
#include <memory>

#include "Level.h"
#include "Texture2D.h"

namespace dae {
	class Scene;
}

namespace qbert
{
	class LevelFactory
	{
	public:
		static std::weak_ptr<Level> CreateLevel(dae::Scene& scene, int discAmount = 2, float scale = 1, int levelSize = 7, const std::weak_ptr<Score>& score = std::weak_ptr<Score>());

	private:
		static void CreateDisc(std::shared_ptr<Level> level, std::shared_ptr<dae::GameObject> levelObject, int cubeWidth, int cubeHeight, int heightIncrement, int startHeight, std::shared_ptr<dae::Texture2D> discTexture, int discWidth, int discHeight, int row, int col);

		static int m_CubeHeightOffset;
	};
}
