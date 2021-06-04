#include "LevelFactory.h"

#include <numeric>

#include "Disc.h"
#include "GameObject.h"
#include "LevelCube.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Texture2D.h"

int qbert::LevelFactory::m_CubeHeightOffset = 8;

const std::weak_ptr<qbert::Level> qbert::LevelFactory::CreateLevel(dae::Scene& scene, LevelType type, int discAmount, int scale, int levelSize)
{
	int windowWidth = 0;
	int windowHeight = 0;
	dae::Renderer::GetInstance().GetWindowSize(windowWidth, windowHeight);

	auto level = std::make_shared<Level>(levelSize);
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
			auto levelCube = std::make_shared<LevelCube>(type, level, row, col, glm::vec3(16 * scale, 8 * scale, 0), glm::vec3(24 * scale, 20 * scale, 0), glm::vec3(8 * scale, 20 * scale, 0));
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
	
	std::vector<int> leftDiscs(levelSize - 1);
	std::vector<int> rightDiscs(levelSize - 1);
	std::iota(std::begin(leftDiscs), std::end(leftDiscs), 0);
	std::iota(std::begin(rightDiscs), std::end(rightDiscs), 0);
	bool left = true;
	for (int i = 0; i < discAmount; ++i)
	{
		int index = rand() % (left ? leftDiscs.size() : rightDiscs.size());
		int row = left ? leftDiscs[index] : rightDiscs[index];
		int col = left ? -1 : row + 1;

		auto disc = std::make_shared<Disc>(level, row, col, glm::vec3(8 * scale, 6 * scale, 0));

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

		level->SetLevel(disc, row, col);

		RemoveUsedDisc(left ? leftDiscs : rightDiscs, row);
		left = !left;
	}

	scene.Add(levelObject);
	return level;
}

void qbert::LevelFactory::RemoveUsedDisc(std::vector<int>& discs, int row)
{
	discs.erase(std::remove(std::begin(discs), std::end(discs), row), std::end(discs));
}
