#include "MiniginPCH.h"
#include "Minigin.h"
#include <vld.h>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>

#include "FPS.h"
#include "GameObject.h"
#include "LoggingSoundSystem.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "SimpleSoundSystem.h"
#include "TextComponent.h"
#include "GameTime.h"

using namespace std;
using namespace std::chrono;

dae::Minigin::Minigin()
{
	srand(static_cast<unsigned>(time(nullptr)));
}

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
	SceneManager::GetInstance().CreateScene("Demo", [](Scene& scene)
		{
			//Background
			auto go = std::make_shared<GameObject>();
			auto renderComponent = std::make_shared<RenderComponent>("background.jpg");
			go->AddComponent(renderComponent);
			scene.Add(go);

			//DAE Logo
			go = std::make_shared<GameObject>();
			renderComponent = std::make_shared<RenderComponent>("logo.png");
			go->AddComponent(renderComponent);
			go->GetTransform().SetWorldPosition(216, 180, 0);
			scene.Add(go);

			//Title
			go = std::make_shared<GameObject>();
			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
			auto textComponent = std::make_shared<TextComponent>(font, "Programming 4 Assignment");
			go->AddComponent(textComponent);
			go->GetTransform().SetWorldPosition(80, 20, 0);
			scene.Add(go);

			//FPS
			go = std::make_shared<GameObject>();
			textComponent = std::make_shared<TextComponent>(font, "", Color{ 255, 255, 0 });
			go->AddComponent(textComponent);
			auto fps = std::make_shared<FPS>();
			go->AddComponent(fps);
			scene.Add(go);
		});
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
	auto pLoggingSoundSystem = new LoggingSoundSystem(new SimpleSoundSystem());
	ServiceLocator::RegisterSoundSystem(pLoggingSoundSystem);

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& time = GameTime::GetInstance();

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

	delete pLoggingSoundSystem;

	Cleanup();
}
