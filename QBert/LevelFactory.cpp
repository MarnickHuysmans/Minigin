#include "LevelFactory.h"
#include "Disc.h"
#include "GameObject.h"
#include "LevelCube.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Texture2D.h"
#include "LevelEnums.h"

int qbert::LevelFactory::m_CubeHeightOffset = 8;

std::weak_ptr<qbert::Level> qbert::LevelFactory::CreateLevel(dae::Scene& scene, int discAmount, int scale, int levelSize)
{
	int windowWidth = 0;
	int windowHeight = 0;
	dae::Renderer::GetInstance().GetWindowSize(windowWidth, windowHeight);

	auto level = std::make_shared<Level>(levelSize, discAmount);
	auto levelObject = std::make_shared<dae::GameObject>();
	levelObject->AddComponent(level);

	//LevelCubes
	auto cubeTexture1 = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Cube1.png");
	auto cubeTexture2 = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Cube2.png");
	auto cubeTexture3 = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Cube3.png");

	int cubeWidth = 0;
	int cubeHeight = 0;
	cubeTexture1->GetTextureSize(cubeWidth, cubeHeight);

	int heightIncrement = cubeHeight - m_CubeHeightOffset;
	int levelHeight = (heightIncrement * levelSize + m_CubeHeightOffset) * scale;
	int heightRemaining = windowHeight - levelHeight;
	int startHeight = windowHeight - levelHeight - heightRemaining / 4;

	int windowMiddle = windowWidth / 2;

	for (int row = 0; row < levelSize; ++row)
	{
		for (int col = 0; col <= row; ++col)
		{
			auto levelCube = std::make_shared<LevelCube>(LevelType::Single, level, row, col, glm::vec3(16, 8, 0), glm::vec3(24, 20, 0), glm::vec3(8, 20, 0));
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

			level->SetLevel(levelCube, row, col);
			levelCube->AddObserver(level);
		}
	}

	//Discs
	auto discTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Disc.png");
	int discWidth = 0;
	int discHeight = 0;
	discTexture->GetTextureSize(discWidth, discHeight);

	for (int row = 0; row < levelSize - 1; ++row)
	{
		CreateDisc(scale, level, levelObject, cubeWidth, cubeHeight, heightIncrement, startHeight, windowMiddle, discTexture, discWidth, discHeight, row, -1);
		CreateDisc(scale, level, levelObject, cubeWidth, cubeHeight, heightIncrement, startHeight, windowMiddle, discTexture, discWidth, discHeight, row, row + 1);
	}

	scene.Add(levelObject);
	return level;
}

void qbert::LevelFactory::CreateDisc(int scale, std::shared_ptr<Level> level, std::shared_ptr<dae::GameObject> levelObject, int cubeWidth, int cubeHeight, int heightIncrement, int startHeight, int windowMiddle, std::shared_ptr<dae::Texture2D> discTexture, int discWidth, int discHeight, int row, int col)
{
	auto disc = std::make_shared<Disc>(level, row, col, glm::vec3(8, 6, 0));

	auto renderComponent = std::make_shared<dae::RenderComponent>();
	renderComponent->SetTexture(discTexture);

	auto discObject = std::make_shared<dae::GameObject>();
	discObject->AddComponent(renderComponent);
	discObject->AddComponent(disc);

	float x = static_cast<float>(windowMiddle - (row + 1) * cubeWidth * scale / 2 + col * cubeWidth * scale + (cubeWidth - discWidth) * scale / 2);
	float y = static_cast<float>(startHeight + (heightIncrement * row + cubeHeight - discHeight) * scale);

	discObject->GetTransform().SetWorldPosition(x, y, 0);
	discObject->GetTransform().SetWorldScale(static_cast<float>(scale), static_cast<float>(scale));
	levelObject->AddChild(discObject);

	discObject->SetActive(false);

	level->SetLevel(disc, row, col);
}
