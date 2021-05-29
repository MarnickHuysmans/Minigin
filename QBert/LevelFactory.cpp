#include "LevelFactory.h"

#include "Renderer.h"

int qbert::LevelFactory::m_CubeHeightOffset = 8;

const std::weak_ptr<qbert::Level> qbert::LevelFactory::CreateLevel(dae::Scene& scene, LevelType type, int discAmount, float scale, int levelSize)
{
	int windowWidth;
	int windowHeight;
	dae::Renderer::GetInstance().GetWindowSize(windowWidth, windowHeight);

	
}
