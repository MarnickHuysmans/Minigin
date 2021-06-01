#include <vld.h>

#include "Minigin.h"

#include "GameObject.h"
#include "Lives.h"
#include "Qbert.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Score.h"
#include "TextComponent.h"
#include "InputManager.h"
#include "LevelFactory.h"
#include "ServiceLocator.h"

class QBertGame : public dae::Minigin
{
public:

	void LoadGame() const override;
};

void QBertGame::LoadGame() const
{
	Minigin::LoadGame();

	auto& scene = *dae::SceneManager::GetInstance().GetScene("Demo");

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//qBert1
	auto qbertObject = std::make_shared<dae::GameObject>();
	auto qBert = std::make_shared<qbert::Qbert>();
	qBert->SetButtons(dae::ControllerButton::ShoulderRight, dae::ControllerButton::ButtonA, dae::ControllerButton::ButtonB, dae::ControllerButton::ButtonX, dae::ControllerButton::ButtonY);
	qbertObject->AddComponent(qBert);
	scene.Add(qbertObject);

	//Lives display1
	auto go = std::make_shared<dae::GameObject>();
	auto textComponent = std::make_shared<dae::TextComponent>(font, "", dae::Color{ 255,0,255 });
	go->AddComponent(textComponent);
	auto lives = std::make_shared<qbert::Lives>(qBert.get());
	go->AddComponent(lives);
	qBert->AddObserver(lives.get());
	go->GetTransform().SetWorldPosition(540, 200, 0);
	scene.Add(go);

	//Score1
	go = std::make_shared<dae::GameObject>();
	textComponent = std::make_shared<dae::TextComponent>(font, "", dae::Color{ 255,0,255 });
	go->AddComponent(textComponent);
	auto score = std::make_shared<qbert::Score>();
	go->AddComponent(score);
	qBert->AddObserver(score.get());
	go->GetTransform().SetWorldPosition(0, 200, 0);
	scene.Add(go);

	//qBert2
	qbertObject = std::make_shared<dae::GameObject>();
	qBert = std::make_shared<qbert::Qbert>();
	qBert->SetButtons(dae::KeyboardCode::SDL_SCANCODE_E, dae::KeyboardCode::SDL_SCANCODE_W, dae::KeyboardCode::SDL_SCANCODE_A, dae::KeyboardCode::SDL_SCANCODE_S, dae::KeyboardCode::SDL_SCANCODE_D);
	qbertObject->AddComponent(qBert);
	scene.Add(qbertObject);

	//Lives display2
	go = std::make_shared<dae::GameObject>();
	textComponent = std::make_shared<dae::TextComponent>(font, "", dae::Color{ 0,255,255 });
	go->AddComponent(textComponent);
	lives = std::make_shared<qbert::Lives>(qBert.get());
	go->AddComponent(lives);
	qBert->AddObserver(lives.get());
	go->GetTransform().SetWorldPosition(540, 300, 0);
	scene.Add(go);

	//Score2
	go = std::make_shared<dae::GameObject>();
	textComponent = std::make_shared<dae::TextComponent>(font, "", dae::Color{ 0,255,255 });
	go->AddComponent(textComponent);
	score = std::make_shared<qbert::Score>();
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
			}
			if (ImGui::Button("Co-op"))
			{
			}
			if (ImGui::Button("Versus"))
			{
			}
		}, "Menu", nullptr, flags);
	uiComponent->SetPosition({ 100, 100 });
	go->AddComponent(uiComponent);
	uiComponent = std::make_shared<dae::UIComponent>([]()
		{
			ImGui::Text("Player1: 4 face buttons and right shoulder");
			ImGui::Text("Player2: WASD and E");
		}, "Controls", nullptr, flags);
	uiComponent->SetPosition({ 100, 400 });
	go->AddComponent(uiComponent);
	scene.Add(go);

	qbert::LevelFactory::CreateLevel(scene, qbert::LevelType::Single, 2, 2);

	dae::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sound/GameStartMusic.wav");
}

int main(int, char* [])
{
	QBertGame game{};
	game.Run();
	return 0;
}
