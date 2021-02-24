#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>

dae::InputManager::InputManager()
{
	ZeroMemory(&m_PreviousControllerState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
	m_CurrentKeyboardState = SDL_GetKeyboardState(nullptr);
}

bool dae::InputManager::ProcessInput()
{
	m_PreviousControllerState = m_CurrentControllerState;
	ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentControllerState);

	UpdatePreviousKeyboardState();

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN)
		{
			m_KeysDown.push_back(e.key.keysym.scancode);
		}
		else if (e.type == SDL_KEYUP)
		{
			m_KeysUp.push_back(e.key.keysym.scancode);
		}
	}
	
	HandleInput();
	return true;
}

bool dae::InputManager::IsPressed(ControllerButton button) const
{
	return m_CurrentControllerState.Gamepad.wButtons & static_cast<int>(button);
}

bool dae::InputManager::IsDown(ControllerButton button) const
{
	return !(m_PreviousControllerState.Gamepad.wButtons & static_cast<int>(button)) && m_CurrentControllerState.Gamepad.wButtons & static_cast<int>(button);
}

bool dae::InputManager::IsUp(ControllerButton button) const
{
	return m_PreviousControllerState.Gamepad.wButtons & static_cast<int>(button) && !(m_CurrentControllerState.Gamepad.wButtons & static_cast<int>(button));
}

bool dae::InputManager::IsPressed(SDL_Scancode key) const
{
	return m_CurrentKeyboardState[key];
}

bool dae::InputManager::IsDown(SDL_Scancode key) const
{
	return !m_PreviousKeyboardState[key] && m_CurrentKeyboardState[key];
}

bool dae::InputManager::IsUp(SDL_Scancode key) const
{
	return m_PreviousKeyboardState[key] && !m_CurrentKeyboardState[key];
}

void dae::InputManager::AddCommand(std::unique_ptr<Command>& command, ControllerButton button, InputState inputState)
{
	m_ControllerCommandMap[ControllerKey{ button, inputState }] = std::move(command);
}

void dae::InputManager::AddCommand(std::unique_ptr<Command>& command, SDL_Scancode key, InputState inputState)
{
	m_KeyboardCommandMap[KeyboardKey{ key, inputState }] = std::move(command);
}

void dae::InputManager::HandleInput()
{
	for (auto& commandPair : m_ControllerCommandMap)
	{
		switch (commandPair.first.second)
		{
		case InputState::Down:
			if (IsDown(commandPair.first.first))
			{
				commandPair.second->Execute();
			}
			break;
		case InputState::Up:
			if (IsUp(commandPair.first.first))
			{
				commandPair.second->Execute();
			}
			break;
		case InputState::Hold:
			if (IsPressed(commandPair.first.first))
			{
				commandPair.second->Execute();
			}
		}
	}
}

void dae::InputManager::UpdatePreviousKeyboardState()
{
	for (auto down : m_KeysDown)
	{
		m_PreviousKeyboardState[down] = true;
	}
	m_KeysDown.clear();
	for (auto up : m_KeysUp)
	{
		m_PreviousKeyboardState[up] = false;
	}
	m_KeysUp.clear();
}
