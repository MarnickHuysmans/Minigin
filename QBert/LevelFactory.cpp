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

std::weak_ptr<qbert::Level> qbert::LevelFactory::CreateLevel(dae::Scene& scene, const std::weak_ptr<Score>& score,
                                                             LevelSettings levelSettings)
{
	int windowWidth = 0;
	int windowHeight = 0;
	dae::Renderer::GetInstance().GetWindowSize(windowWidth, windowHeight);

	auto level = std::make_shared<Level>(levelSettings.levelSize, levelSettings.discAmount, score);
	auto levelObject = std::make_shared<dae::GameObject>();
	levelObject->AddComponent(level);

	int levelMiddleX = windowWidth / 2;
	int levelMiddleY = windowHeight / 2;

	levelObject->GetTransform().SetWorldPosition(static_cast<float>(levelMiddleX), static_cast<float>(levelMiddleY), 0);
	levelObject->GetTransform().SetWorldScale(levelSettings.scale, levelSettings.scale);

	//LevelCubes
	auto cubeTexture1 = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Cube1.png");
	auto cubeTexture2 = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Cube2.png");
	auto cubeTexture3 = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Cube3.png");

	int cubeWidth = 0;
	int cubeHeight = 0;
	cubeTexture1->GetTextureSize(cubeWidth, cubeHeight);

	int heightIncrement = cubeHeight - m_CubeHeightOffset;
	int levelHeight = heightIncrement * levelSettings.levelSize + m_CubeHeightOffset;
	int startHeight = -levelHeight / 2;

	for (int row = 0; row < levelSettings.levelSize; ++row)
	{
		for (int col = 0; col <= row; ++col)
		{
			auto levelCube = std::make_shared<LevelCube>(LevelType::Single, level, row, col, glm::vec3(16, 8, 0),
			                                             glm::vec3(24, 20, 0), glm::vec3(8, 20, 0));
			levelCube->SetTexture(cubeTexture1, 0);
			levelCube->SetTexture(cubeTexture2, 1);
			levelCube->SetTexture(cubeTexture3, 2);

			auto renderComponent = std::make_shared<dae::RenderComponent>();

			auto cubeObject = std::make_shared<dae::GameObject>();
			cubeObject->AddComponent(renderComponent);
			cubeObject->AddComponent(levelCube);


			float x = static_cast<float>((row + 1) * -cubeWidth / 2 + col * cubeWidth);
			float y = static_cast<float>(startHeight + heightIncrement * row);

			cubeObject->GetTransform().SetLocalPosition(x, y, 0);
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

	for (int row = 0; row < levelSettings.levelSize - 1; ++row)
	{
		CreateDisc(level, levelObject, cubeWidth, cubeHeight, heightIncrement, startHeight, discTexture, discWidth,
		           discHeight, row, -1);
		CreateDisc(level, levelObject, cubeWidth, cubeHeight, heightIncrement, startHeight, discTexture, discWidth,
		           discHeight, row, row + 1);
	}

	scene.Add(levelObject);
	return level;
}

void qbert::LevelFactory::CreateDisc(std::shared_ptr<Level> level, std::shared_ptr<dae::GameObject> levelObject,
                                     int cubeWidth, int cubeHeight, int heightIncrement, int startHeight,
                                     std::shared_ptr<dae::Texture2D> discTexture, int discWidth, int discHeight,
                                     int row, int col)
{
	auto disc = std::make_shared<Disc>(level, row, col, glm::vec3(8, 6, 0));

	auto renderComponent = std::make_shared<dae::RenderComponent>();
	renderComponent->SetTexture(discTexture);

	auto discObject = std::make_shared<dae::GameObject>();
	discObject->AddComponent(renderComponent);
	discObject->AddComponent(disc);

	float x = static_cast<float>((row + 1) * -cubeWidth / 2 + col * cubeWidth + (cubeWidth - discWidth) / 2);
	float y = static_cast<float>(startHeight + heightIncrement * row + cubeHeight - discHeight);

	discObject->GetTransform().SetLocalPosition(x, y, 0);
	levelObject->AddChild(discObject);

	discObject->SetActive(false);

	level->SetLevel(disc, row, col);
	disc->AddObserver(level);
}
