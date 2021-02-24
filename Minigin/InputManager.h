#pragma once
#include <map>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include <XInput.h>
#include "Command.h"
#include "Singleton.h"

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
		ButtonA = XINPUT_GAMEPAD_A,
		ButtonB = XINPUT_GAMEPAD_B,
		ButtonX = XINPUT_GAMEPAD_X,
		ButtonY = XINPUT_GAMEPAD_Y,
		ButtonBack = XINPUT_GAMEPAD_BACK,
		ButtonStart = XINPUT_GAMEPAD_START,
		DpadUp = XINPUT_GAMEPAD_DPAD_UP,
		DpadDown = XINPUT_GAMEPAD_DPAD_DOWN,
		DpadLeft = XINPUT_GAMEPAD_DPAD_LEFT,
		DpadRight = XINPUT_GAMEPAD_DPAD_RIGHT,
		ShoulderLeft = XINPUT_GAMEPAD_LEFT_SHOULDER,
		ShoulderRight = XINPUT_GAMEPAD_RIGHT_SHOULDER
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
		void UpdatePreviousKeyboardState();

		XINPUT_STATE m_PreviousControllerState{};
		XINPUT_STATE m_CurrentControllerState{};
		bool m_PreviousKeyboardState[SDL_NUM_SCANCODES]{ false };
		const Uint8* m_CurrentKeyboardState{};
		std::vector<SDL_Scancode> m_KeysDown{10};
		std::vector<SDL_Scancode> m_KeysUp{10};
		std::map<ControllerKey, std::unique_ptr<Command>> m_ControllerCommandMap{};
		std::map<KeyboardKey, std::unique_ptr<Command>> m_KeyboardCommandMap{};
	};
}