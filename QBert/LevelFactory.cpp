#include "LevelFactory.h"

#include "GameObject.h"
#include "LevelCube.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Texture2D.h"

int qbert::LevelFactory::m_CubeHeightOffset = 8;

const std::weak_ptr<qbert::Level> qbert::LevelFactory::CreateLevel(dae::Scene& scene, LevelType type, int, int scale, int levelSize)
{
	int windowWidth = 0;
	int windowHeight = 0;
	dae::Renderer::GetInstance().GetWindowSize(windowWidth, windowHeight);

	auto level = std::make_shared<Level>(levelSize);
	auto levelObject = std::make_shared<dae::GameObject>();
	levelObject->AddComponent(level);

	auto cubeTexture1 = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Cube1.png");
	auto cubeTexture2 = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Cube2.png");
	auto cubeTexture3 = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Cube3.png");

	int cubeWidth = 0;
	int cubeHeight = 0;
	cubeTexture1->GetTextureSize(cubeWidth, cubeHeight);

	int heightIncrement = cubeHeight - m_CubeHeightOffset;
	int levelHeight = (heightIncrement * levelSize + m_CubeHeightOffset) * scale;
	int startHeight = windowHeight / 2 - levelHeight / 2;

	int windowMiddle = windowWidth / 2;

	for (int row = 0; row < levelSize; ++row)
	{
		for (int col = 0; col <= row; ++col)
		{
			auto levelCube = std::make_shared<LevelCube>(type, level, row, col, glm::vec2(16 * scale, 8 * scale), glm::vec2(24 * scale, 20 * scale), glm::vec2(8 * scale, 20 * scale));
			levelCube->SetTexture(cubeTexture1, 0);
			levelCube->SetTexture(cubeTexture2, 1);
			levelCube->SetTexture(cubeTexture3, 2);

			auto renderComponent = std::make_shared<dae::RenderComponent>();

			auto cubeObject = std::make_shared<dae::GameObject>();
			cubeObject->AddComponent(renderComponent);
			cubeObject->AddComponent(levelCube);

			
			float x = static_cast<float>(windowMiddle - (row + 1) * cubeWidth * scale / 2 + col * cubeWidth * scale);
			float y = static_cast<float>(startHeight + heightIncrement * row * scale);
			
			cubeObject->GetTransform().SetWorldPosition(x, y, 0);
			cubeObject->GetTransform().SetWorldScale(static_cast<float>(scale), static_cast<float>(scale));
			levelObject->AddChild(cubeObject);
			//scene.Add(cubeObject);

			level->SetLevel(levelCube, row, col);
		}
	}

	scene.Add(levelObject);
	return level;
}
