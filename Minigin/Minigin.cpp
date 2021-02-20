#include "MiniginPCH.h"
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>

#include "FPS.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Scene.h"
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

	auto go = std::make_shared<GameObject>();
	auto* renderComponent = new RenderComponent{};
	renderComponent->SetTexture("background.jpg");
	go->AddComponent(renderComponent);
	scene.Add(go);

	go = std::make_shared<GameObject>();
	renderComponent = new RenderComponent{};
	renderComponent->SetTexture("logo.png");
	go->AddComponent(renderComponent);
	go->SetPosition(216, 180);
	scene.Add(go);

	go = std::make_shared<GameObject>();
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto* textComponent = new TextComponent{ font, "Programming 4 Assignment" };
	go->AddComponent(textComponent);
	go->SetPosition(80, 20);
	scene.Add(go);

	go = std::make_shared<GameObject>();
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	textComponent = new TextComponent{ font, "", {255, 255, 0}};
	go->AddComponent(textComponent);
	auto* fps = new FPS{};
	go->AddComponent(fps);
	go->SetPosition(0, 0);
	scene.Add(go);
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
