#pragma once
struct SDL_Window;
namespace dae
{
	class Minigin
	{
	public:
		Minigin() = default;
		virtual ~Minigin() = default;
		void Initialize();
		virtual void LoadGame() const;
		void Cleanup();
		void Run();
	private:
		SDL_Window* m_Window{};
	};

	// To be defined in CLIENT
	//Minigin* CreateApplication();
}