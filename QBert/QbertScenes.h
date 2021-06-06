#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "FPS.h"
#include "GameDone.h"
#include "GameOver.h"
#include "LevelEnums.h"
#include "LevelFactory.h"
#include "Lives.h"
#include "PlayerInput.h"
#include "QbertDeleteThis.h"
#include "QbertFactory.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "ScoreDeleteThis.h"
#include "ServiceLocator.h"
#include "TextComponent.h"
#include "UIComponent.h"

namespace qbert
{
	class StartScene
	{
	public:
		void operator() (dae::Scene& scene) const
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
			go->GetTransform().SetWorldPosition(216, 180, 0);
			scene.Add(go);

			//Title
			go = std::make_shared<dae::GameObject>();
			auto textComponent = std::make_shared<dae::TextComponent>(font, "Programming 4 Assignment");
			go->AddComponent(textComponent);
			go->GetTransform().SetWorldPosition(80, 20, 0);
			scene.Add(go);

			//FPS
			go = std::make_shared<dae::GameObject>();
			textComponent = std::make_shared<dae::TextComponent>(font, "", dae::Color{ 255, 255, 0 });
			go->AddComponent(textComponent);
			auto fps = std::make_shared<dae::FPS>();
			go->AddComponent(fps);
			scene.Add(go);

			//qBert1
			auto qbertObject = std::make_shared<dae::GameObject>();
			auto qBert = std::make_shared<QbertDeleteThis>();
			qBert->SetButtons(dae::ControllerButton::ShoulderRight, dae::ControllerButton::ButtonA, dae::ControllerButton::ButtonB, dae::ControllerButton::ButtonX, dae::ControllerButton::ButtonY);
			qbertObject->AddComponent(qBert);
			scene.Add(qbertObject);

			//Lives display1
			go = std::make_shared<dae::GameObject>();
			textComponent = std::make_shared<dae::TextComponent>(font, "", dae::Color{ 255,0,255 });
			go->AddComponent(textComponent);
			auto lives = std::make_shared<Lives>(qBert.get());
			go->AddComponent(lives);
			qBert->AddObserver(lives.get());
			go->GetTransform().SetWorldPosition(540, 200, 0);
			scene.Add(go);

			//Score1
			go = std::make_shared<dae::GameObject>();
			textComponent = std::make_shared<dae::TextComponent>(font, "", dae::Color{ 255,0,255 });
			go->AddComponent(textComponent);
			auto score = std::make_shared<ScoreDeleteThis>();
			go->AddComponent(score);
			qBert->AddObserver(score.get());
			go->GetTransform().SetWorldPosition(0, 200, 0);
			scene.Add(go);

			//qBert2
			qbertObject = std::make_shared<dae::GameObject>();
			qBert = std::make_shared<QbertDeleteThis>();
			qBert->SetButtons(dae::KeyboardCode::SDL_SCANCODE_E, dae::KeyboardCode::SDL_SCANCODE_W, dae::KeyboardCode::SDL_SCANCODE_A, dae::KeyboardCode::SDL_SCANCODE_S, dae::KeyboardCode::SDL_SCANCODE_D);
			qbertObject->AddComponent(qBert);
			scene.Add(qbertObject);

			//Lives display2
			go = std::make_shared<dae::GameObject>();
			textComponent = std::make_shared<dae::TextComponent>(font, "", dae::Color{ 0,255,255 });
			go->AddComponent(textComponent);
			lives = std::make_shared<Lives>(qBert.get());
			go->AddComponent(lives);
			qBert->AddObserver(lives.get());
			go->GetTransform().SetWorldPosition(540, 300, 0);
			scene.Add(go);

			//Score2
			go = std::make_shared<dae::GameObject>();
			textComponent = std::make_shared<dae::TextComponent>(font, "", dae::Color{ 0,255,255 });
			go->AddComponent(textComponent);
			score = std::make_shared<ScoreDeleteThis>();
			go->AddComponent(score);
			qBert->AddObserver(score.get());
			go->GetTransform().SetWorldPosition(0, 300, 0);
			scene.Add(go);

			//UI
			go = std::make_shared<dae::GameObject>();
			ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
			auto uiComponent = std::make_shared<dae::UIComponent>([]()
				{
					if (ImGui::Button("Single Player"))
					{
						dae::SceneManager::GetInstance().SwitchScene("SingleScene");
					}
					if (ImGui::Button("Co-op"))
					{
						dae::SceneManager::GetInstance().SwitchScene("CoopScene");
					}
					if (ImGui::Button("Versus"))
					{
						dae::SceneManager::GetInstance().SwitchScene("VersusScene");
					}
				}, "Menu", nullptr, flags);
			uiComponent->SetPosition({ 50, 100 });
			go->AddComponent(uiComponent);

			uiComponent = std::make_shared<dae::UIComponent>([]()
				{
					ImGui::Text("Player1: 4 face buttons and right shoulder");
					ImGui::Text("Player2: WASD and E");
				}, "Controls", nullptr, flags);
			uiComponent->SetPivot({ .5f, 1 });
			uiComponent->SetPosition({ 320, 430 });
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
			uiComponent->SetPosition({ 590, 100 });
			uiComponent->SetPivot({ 1,0 });
			go->AddComponent(uiComponent);

			scene.Add(go);

			LevelFactory::CreateLevel(scene, 2, 2);
		}
	};

	class SingleScene
	{
	public:
		void operator() (dae::Scene& scene) const
		{
			auto weakLevel = LevelFactory::CreateLevel(scene, 2, 2);

			if (weakLevel.expired())
			{
				return;
			}
			auto level = weakLevel.lock();

			auto weakQbert = QbertFactory::CreateQbert(level->GetTopCube(), dae::Player::Player1);

			ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
			auto go = std::make_shared<dae::GameObject>();
			auto uiComponent = std::make_shared<dae::UIComponent>([]()
				{
					ImGui::Text("----------Game Over----------");
					ImGui::SetCursorPosX(75);
					if (ImGui::Button("Main Menu"))
					{
						dae::SceneManager::GetInstance().SwitchScene("StartScene");
					}
				}, "Game Over", nullptr, flags);
			uiComponent->SetPivot({ 0.5f, 0.5f });
			uiComponent->SetPosition({ 320, 240 });
			go->AddComponent(uiComponent);
			auto gameOver = std::make_shared<GameOver>();
			go->AddComponent(gameOver);

			if (weakQbert.expired())
			{
				return;
			}
			auto qbert = weakQbert.lock();
			qbert->AddObserver(gameOver);

			scene.Add(go);

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
			uiComponent->SetPivot({ 0.5f, 0.5f });
			uiComponent->SetPosition({ 320, 240 });
			go->AddComponent(uiComponent);
			auto gameDone = std::make_shared<GameDone>();
			go->AddComponent(gameDone);

			level->AddObserver(gameDone);

			scene.Add(go);
		}
	};

	class CoopScene
	{
	public:
		void operator() (dae::Scene& scene) const
		{
			LevelFactory::CreateLevel(scene, 2, 2);
		}
	};

	class VersusScene
	{
	public:
		void operator() (dae::Scene& scene) const
		{
			LevelFactory::CreateLevel(scene, 2, 2);
		}
	};
}
