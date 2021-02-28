#pragma once
#include <map>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include "Command.h"
#include "Controller.h"
#include "Singleton.h"

struct _XINPUT_STATE;

namespace dae
{	
	enum class InputState
	{
		Down,
		Up,
		Hold
	};

	enum class Player : int
	{
		Player1 = 0,
		Player2 = 1,
		Player3 = 2,
		Player4 = 3
	};

	using ControllerKey = std::pair<ControllerButton, InputState>;
	using ControllerCommandMap = std::map<Player, std::map<ControllerKey, std::unique_ptr<Command>>>;
	using KeyboardKey = std::pair<SDL_Scancode, InputState>;
	using KeyboardCommandMap = std::map<KeyboardKey, std::unique_ptr<Command>>;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();

		bool ProcessInput();
		bool IsPressed(SDL_Scancode key) const;
		bool IsDown(SDL_Scancode key) const;
		bool IsUp(SDL_Scancode key) const;
		void AddCommand(std::unique_ptr<Command>& command, Player player, ControllerButton button, InputState inputState = InputState::Down);
		void AddCommand(std::unique_ptr<Command>& command, SDL_Scancode key, InputState inputState = InputState::Down);
		void UpdateConnectedControllers();

	private:
		void HandleInput();
		void HandleControllerInput();
		void HandleKeyboardInput();
		void UpdateControllerStates();
		void UpdatePreviousKeyboardState();

		const static DWORD m_MaxControllers = 4;
		Controller m_Controllers[m_MaxControllers]{};
		bool m_PreviousKeyboardState[SDL_NUM_SCANCODES]{ false };
		const Uint8* m_CurrentKeyboardState{};
		std::vector<SDL_Scancode> m_KeysDown{ 10 };
		std::vector<SDL_Scancode> m_KeysUp{ 10 };
		ControllerCommandMap m_ControllerCommandMap{};
		KeyboardCommandMap m_KeyboardCommandMap{};
	};
}