#pragma once
#include "Core.h"
struct SDL_Window;
namespace dae
{
	class MINIGIN_API Minigin
	{
	public:
		Minigin() = default;
		virtual ~Minigin() = default;
		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();
	private:
		SDL_Window* m_Window{};
	};

	// To be defined in CLIENT
	//Minigin* CreateApplication();
}