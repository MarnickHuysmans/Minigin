#include "QbertScenes.h"

#include <algorithm>
#include <fstream>


#include "EnemySpawner.h"
#include "GameObject.h"
#include "FPS.h"
#include "GameDone.h"
#include "GameOver.h"
#include "LevelFactory.h"
#include "PlayerInput.h"
#include "QbertFactory.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "UIComponent.h"
#include "Score.h"
#include "Movement.h"

qbert::GameScene::GameType qbert::GameScene::m_GameType = GameType::Single;

void qbert::StartScene::operator()(dae::Scene& scene) const
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//Background
	auto go = std::make_shared<dae::GameObject>();
	auto renderComponent = std::make_shared<dae::RenderComponent>("background.jpg");
	go->AddComponent(renderComponent);
	scene.Add(go);

	//DAE Logo
	go = std::make_shared<dae::GameObject>();
	renderComponent = std::make_shared<dae::RenderComponent>("logo.png");
	go->AddComponent(renderComponent);
	go->GetTransform().SetWorldPosition(216, 100, 0);
	scene.Add(go);

	//Title
	go = std::make_shared<dae::GameObject>();
	auto textComponent = std::make_shared<dae::TextComponent>(font, "Programming 4 Assignment");
	go->AddComponent(textComponent);
	go->GetTransform().SetWorldPosition(80, 20, 0);
	scene.Add(go);

	//FPS
	go = std::make_shared<dae::GameObject>();
	textComponent = std::make_shared<dae::TextComponent>(font, "", dae::Color{255, 255, 0});
	go->AddComponent(textComponent);
	auto fps = std::make_shared<dae::FPS>();
	go->AddComponent(fps);
	scene.Add(go);

	//UI
	go = std::make_shared<dae::GameObject>();
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings;
	auto uiComponent = std::make_shared<dae::UIComponent>([]()
	{
		if (ImGui::Button("Single Player"))
		{
			GameScene::SetGameType(GameScene::GameType::Single);
			dae::SceneManager::GetInstance().SwitchScene("GameScene");
		}
		if (ImGui::Button("Co-op"))
		{
			GameScene::SetGameType(GameScene::GameType::Coop);
			dae::SceneManager::GetInstance().SwitchScene("GameScene");
		}
		if (ImGui::Button("Versus"))
		{
			GameScene::SetGameType(GameScene::GameType::Versus);
			dae::SceneManager::GetInstance().SwitchScene("GameScene");
		}
	}, "Menu", nullptr, flags);
	uiComponent->SetPosition({10, 75});
	go->AddComponent(uiComponent);

	uiComponent = std::make_shared<dae::UIComponent>([]()
	{
		ImGui::Text("Keyboard: WASD & arrow keys");
		ImGui::Text("Controller: DPAD");
		ImGui::Text("Controller 1 and WASD will be used before arrow keys and controller 2");
	}, "Controls", nullptr, flags);
	uiComponent->SetPivot({.5f, 1});
	uiComponent->SetPosition({320, 470});
	go->AddComponent(uiComponent);

	uiComponent = std::make_shared<dae::UIComponent>([]()
	{
		auto player1 = PlayerInput::GetPlayer1();
		auto player2 = PlayerInput::GetPlayer2();

		ImGui::Text("Player 1 Input:");
		if (ImGui::RadioButton("Keyboard##1", player1 == InputType::Keyboard))
		{
			PlayerInput::SetPlayer1(InputType::Keyboard);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Controller##1", player1 == InputType::Controller))
		{
			PlayerInput::SetPlayer1(InputType::Controller);
		}

		ImGui::Text("Player 2 Input:");
		if (ImGui::RadioButton("Keyboard##2", player2 == InputType::Keyboard))
		{
			PlayerInput::SetPlayer2(InputType::Keyboard);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Controller##2", player2 == InputType::Controller))
		{
			PlayerInput::SetPlayer2(InputType::Controller);
		}
	}, "PlayerInput", nullptr, flags);
	uiComponent->SetPosition({630, 75});
	uiComponent->SetPivot({1, 0});
	go->AddComponent(uiComponent);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	renderComponent = std::make_shared<dae::RenderComponent>("Sprites/BigQ.png");
	go->AddComponent(renderComponent);
	go->GetTransform().SetWorldPosition(320, 400, 0);
	go->GetTransform().SetPivot({0.5f, 1});
	go->GetTransform().SetWorldScale(2, 2);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	renderComponent = std::make_shared<dae::RenderComponent>("Sprites/Title.png");
	go->AddComponent(renderComponent);
	go->GetTransform().SetWorldPosition(320, 300, 0);
	go->GetTransform().SetPivot({0.5f, 0});
	go->GetTransform().SetWorldScale(2, 2);
	scene.Add(go);
}

void qbert::GameScene::operator()(dae::Scene& scene) const
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//FPS
	auto go = std::make_shared<dae::GameObject>();
	auto textComponent = std::make_shared<dae::TextComponent>(font, "", dae::Color{255, 255, 0});
	go->AddComponent(textComponent);
	auto fps = std::make_shared<dae::FPS>();
	go->AddComponent(fps);
	scene.Add(go);

	//Score
	go = std::make_shared<dae::GameObject>();
	textComponent = std::make_shared<dae::TextComponent>(font, "0", dae::Color{200, 200, 200});
	go->AddComponent(textComponent);
	auto score = std::make_shared<Score>();
	go->AddComponent(score);
	go->GetTransform().SetPivot({0.5f, 0});
	go->GetTransform().SetWorldPosition({320, 10, 0});
	scene.Add(go);

	std::ifstream levelFile;
	std::string line;
	levelFile.open("../Data/Level.txt");
	LevelSettings levelSettings;
	if (levelFile.is_open())
	{
		int lines = 0;
		while (!levelFile.eof() && lines < 3)
		{
			std::getline(levelFile, line);
			try
			{
				switch (lines)
				{
				case 0:
					levelSettings.levelSize = std::stoi(line);
					break;
				case 1:
					levelSettings.discAmount = std::stoi(line);
					break;
				case 2:
					levelSettings.scale = std::stof(line);
					break;
				}
			}
			catch (...)
			{
			}
			++lines;
		}
		levelFile.close();
	}

	//Level
	auto weakLevel = LevelFactory::CreateLevel(scene, score, levelSettings);
	if (weakLevel.expired())
	{
		return;
	}
	auto level = weakLevel.lock();

	//Qbert Players
	std::vector<std::weak_ptr<Qbert>> players{};
	players.push_back(QbertFactory::CreateQbert(
		scene, m_GameType == GameType::Coop ? level->GetBottomLeftCube() : level->GetTopCube(), dae::Player::Player1,
		font));
	level->AddObserver(players[0]);
	if (m_GameType == GameType::Coop)
	{
		players.push_back(QbertFactory::CreateQbert(scene, level->GetBottomRightCube(), dae::Player::Player2, font));
		level->AddObserver(players[1]);

		if (!players[0].expired() && !players[1].expired())
		{
			auto player1 = players[0].lock();
			auto player2 = players[1].lock();
			player1->AddObserver(player2);
			player2->AddObserver(player1);
		}
	}

	//Game Over UI
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings;
	go = std::make_shared<dae::GameObject>();
	auto uiComponent = std::make_shared<dae::UIComponent>([]()
	{
		ImGui::Text("----------Game Over----------");
		ImGui::SetCursorPosX(75);
		if (ImGui::Button("Main Menu"))
		{
			dae::SceneManager::GetInstance().SwitchScene("StartScene");
		}
	}, "Game Over", nullptr, flags);
	uiComponent->SetPivot({0.5f, 0.5f});
	uiComponent->SetPosition({320, 240});
	go->AddComponent(uiComponent);
	auto gameOver = std::make_shared<GameOver>();
	go->AddComponent(gameOver);
	scene.Add(go);

	//Game Complete UI
	go = std::make_shared<dae::GameObject>();
	uiComponent = std::make_shared<dae::UIComponent>([]()
	{
		ImGui::Text("----------Game Complete----------");
		ImGui::SetCursorPosX(90);
		if (ImGui::Button("Main Menu"))
		{
			dae::SceneManager::GetInstance().SwitchScene("StartScene");
		}
	}, "Game Done", nullptr, flags);
	uiComponent->SetPivot({0.5f, 0.5f});
	uiComponent->SetPosition({320, 240});
	go->AddComponent(uiComponent);
	auto gameDone = std::make_shared<GameDone>();
	go->AddComponent(gameDone);
	level->AddObserver(gameDone);
	scene.Add(go);

	//EnemySpawner
	go = std::make_shared<dae::GameObject>();
	std::vector<std::weak_ptr<Movement>> playerMovements{players.size()};
	std::transform(std::begin(players), std::end(players), std::begin(playerMovements),
	               [](const std::weak_ptr<Qbert>& player)
	               {
		               if (player.expired())
		               {
			               return std::weak_ptr<Movement>();
		               }
		               auto weakGameObject = player.lock()->GetGameObject();
		               if (weakGameObject.expired())
		               {
			               return std::weak_ptr<Movement>();
		               }
		               return weakGameObject.lock()->GetComponent<Movement>();
	               });
	auto enemySpawner = std::make_shared<EnemySpawner>(level, playerMovements, m_GameType == GameType::Versus);
	go->AddComponent(enemySpawner);
	scene.Add(go);
	level->AddObserver(enemySpawner);

	//QbertObservers
	for (auto& player : players)
	{
		if (player.expired())
		{
			continue;
		}
		auto qbert = player.lock();
		qbert->AddObserver(gameOver);
		qbert->AddObserver(enemySpawner);
	}
	for (auto& playerMovement : playerMovements)
	{
		if (playerMovement.expired())
		{
			continue;
		}
		playerMovement.lock()->AddObserver(enemySpawner);
	}
}

void qbert::GameScene::SetGameType(GameType gameType)
{
	m_GameType = gameType;
}
