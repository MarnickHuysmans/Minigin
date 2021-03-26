#include "MiniginPCH.h"
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>

#include "FPS.h"
#include "GameObject.h"
#include "Lives.h"
#include "LoggingSoundSystem.h"
#include "Qbert.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "Score.h"
#include "ServiceLocator.h"
#include "SimpleSoundSystem.h"
#include "TextComponent.h"
#include "Time.h"

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	//Background
	auto go = std::make_shared<GameObject>();
	auto renderComponent = std::make_shared<RenderComponent>("background.jpg");
	go->AddComponent(renderComponent);
	scene.Add(go);

	//DAE Logo
	go = std::make_shared<GameObject>();
	renderComponent = std::make_shared<RenderComponent>("logo.png");
	go->AddComponent(renderComponent);
	go->SetPosition(216, 180);
	scene.Add(go);

	//Title
	go = std::make_shared<GameObject>();
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textComponent = std::make_shared<TextComponent>(font, "Programming 4 Assignment");
	go->AddComponent(textComponent);
	go->SetPosition(80, 20);
	scene.Add(go);

	//FPS
	go = std::make_shared<GameObject>();
	//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	textComponent = std::make_shared<TextComponent>(font, "", SDL_Color{ 255, 255, 0 });
	go->AddComponent(textComponent);
	auto fps = std::make_shared<FPS>();
	go->AddComponent(fps);
	go->SetPosition(0, 0);
	scene.Add(go);

	//UI
	go = std::make_shared<GameObject>();
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
	auto uiComponent = std::make_shared<UIComponent>([]()
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
	uiComponent = std::make_shared<UIComponent>([]()
		{
			ImGui::Text("Player1: 4 face buttons and right shoulder");
			ImGui::Text("Player2: WASD and E");
		}, "Controls", nullptr, flags);
	uiComponent->SetPosition({ 100, 300 });
	go->AddComponent(uiComponent);
	scene.Add(go);

	//qBert1
	auto qbertObject = std::make_shared<GameObject>();
	auto qBert = std::make_shared<qbert::Qbert>();
	qBert->SetButtons(ControllerButton::ShoulderRight, ControllerButton::ButtonA, ControllerButton::ButtonB, ControllerButton::ButtonX, ControllerButton::ButtonY);
	qbertObject->AddComponent(qBert);
	scene.Add(qbertObject);

	//Lives display1
	go = std::make_shared<GameObject>();
	textComponent = std::make_shared<TextComponent>(font, "", SDL_Color{ 255,0,255 });
	go->AddComponent(textComponent);
	auto lives = std::make_shared<qbert::Lives>(qBert.get());
	go->AddComponent(lives);
	qBert->AddObserver(lives.get());
	go->SetPosition(540, 200);
	scene.Add(go);

	//Score1
	go = std::make_shared<GameObject>();
	textComponent = std::make_shared<TextComponent>(font, "", SDL_Color{ 255,0,255 });
	go->AddComponent(textComponent);
	auto score = std::make_shared<qbert::Score>();
	go->AddComponent(score);
	qBert->AddObserver(score.get());
	go->SetPosition(0, 200);
	scene.Add(go);

	//qBert2
	qbertObject = std::make_shared<GameObject>();
	qBert = std::make_shared<qbert::Qbert>();
	qBert->SetButtons(KeyboardSDL::SDL_SCANCODE_E, KeyboardSDL::SDL_SCANCODE_W, KeyboardSDL::SDL_SCANCODE_A, KeyboardSDL::SDL_SCANCODE_S, KeyboardSDL::SDL_SCANCODE_D);
	qbertObject->AddComponent(qBert);
	scene.Add(qbertObject);

	//Lives display2
	go = std::make_shared<GameObject>();
	textComponent = std::make_shared<TextComponent>(font, "", SDL_Color{ 0,255,255 });
	go->AddComponent(textComponent);
	lives = std::make_shared<qbert::Lives>(qBert.get());
	go->AddComponent(lives);
	qBert->AddObserver(lives.get());
	go->SetPosition(540, 300);
	scene.Add(go);

	//Score2
	go = std::make_shared<GameObject>();
	textComponent = std::make_shared<TextComponent>(font, "", SDL_Color{ 0,255,255 });
	go->AddComponent(textComponent);
	score = std::make_shared<qbert::Score>();
	go->AddComponent(score);
	qBert->AddObserver(score.get());
	go->SetPosition(0, 300);
	scene.Add(go);

	ServiceLocator::GetSoundSystem().PlaySound("../Data/Sound/GameStartMusic.wav");
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	// Default SoundService
	ServiceLocator::RegisterSoundSystem(new LoggingSoundSystem(new SimpleSoundSystem()));

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& time = Time::GetInstance();

		sceneManager.Start();

		bool doContinue = true;
		while (doContinue)
		{
			doContinue = input.ProcessInput();
			sceneManager.Update();
			renderer.Render();

			time.Update();
		}
	}

	Cleanup();
}
