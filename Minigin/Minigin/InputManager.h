#pragma once
#include <map>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include "Command.h"
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
	enum class ControllerButton : int
	{
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000,
		ButtonBack = 0x0020,
		ButtonStart = 0x0010,
		DpadUp = 0x0001,
		DpadDown = 0x0002,
		DpadLeft = 0x0004,
		DpadRight = 0x0008,
		ShoulderLeft = 0x0100,
		ShoulderRight = 0x0200,
		LeftThumb = 0x0040,
		RightThumb = 0x0080
	};

	using ControllerKey = std::pair<ControllerButton, InputState>;
	using KeyboardKey = std::pair<SDL_Scancode, InputState>;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();

		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;
		bool IsDown(ControllerButton button) const;
		bool IsUp(ControllerButton button) const;
		bool IsPressed(SDL_Scancode key) const;
		bool IsDown(SDL_Scancode key) const;
		bool IsUp(SDL_Scancode key) const;
		void AddCommand(std::unique_ptr<Command>& command, ControllerButton button, InputState inputState = InputState::Down);
		void AddCommand(std::unique_ptr<Command>& command, SDL_Scancode key, InputState inputState = InputState::Down);

	private:
		void HandleInput();
		void HandleControllerInput();
		void HandleKeyboardInput();
		void UpdateControllerStates();
		void UpdatePreviousKeyboardState();

		_XINPUT_STATE* m_PreviousControllerState;
		_XINPUT_STATE* m_CurrentControllerState;
		bool m_PreviousKeyboardState[SDL_NUM_SCANCODES]{ false };
		const Uint8* m_CurrentKeyboardState{};
		std::vector<SDL_Scancode> m_KeysDown{ 10 };
		std::vector<SDL_Scancode> m_KeysUp{ 10 };
		std::map<ControllerKey, std::unique_ptr<Command>> m_ControllerCommandMap{};
		std::map<KeyboardKey, std::unique_ptr<Command>> m_KeyboardCommandMap{};
	};
}