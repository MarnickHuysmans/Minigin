#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include <XInput.h>

dae::InputManager::InputManager()
{
	m_CurrentKeyboardState = SDL_GetKeyboardState(nullptr);
	UpdateConnectedControllers();
}

bool dae::InputManager::ProcessInput()
{
	UpdateControllerStates();
	UpdatePreviousKeyboardState();

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN)
		{
			m_KeysDown.push_back(static_cast<KeyboardCode>(e.key.keysym.scancode));
		}
		else if (e.type == SDL_KEYUP)
		{
			m_KeysUp.push_back(static_cast<KeyboardCode>(e.key.keysym.scancode));
		}
	}

	HandleInput();
	return true;
}

bool dae::InputManager::IsPressed(ControllerButton button, Player player) const
{
	return m_Controllers[static_cast<int>(player)].IsPressed(button);
}

bool dae::InputManager::IsDown(ControllerButton button, Player player) const
{
	return m_Controllers[static_cast<int>(player)].IsDown(button);
}

bool dae::InputManager::IsUp(ControllerButton button, Player player) const
{
	return m_Controllers[static_cast<int>(player)].IsUp(button);
}

bool dae::InputManager::IsPressed(KeyboardCode key) const
{
	return m_CurrentKeyboardState[static_cast<unsigned int>(key)];
}

bool dae::InputManager::IsDown(KeyboardCode key) const
{
	return !m_PreviousKeyboardState[static_cast<int>(key)] && m_CurrentKeyboardState[static_cast<unsigned int>(key)];
}

bool dae::InputManager::IsUp(KeyboardCode key) const
{
	return m_PreviousKeyboardState[static_cast<int>(key)] && !m_CurrentKeyboardState[static_cast<unsigned int>(key)];
}

void dae::InputManager::AddCommand(const std::function<void()>& command, Player player, ControllerButton button, InputState inputState)
{
	m_ControllerCommandMap[player].insert_or_assign(ControllerKey{ button, inputState }, command);
}

void dae::InputManager::AddCommand(const std::function<void()>& command, KeyboardCode key, InputState inputState)
{
	m_KeyboardCommandMap.insert_or_assign(KeyboardKey{ key, inputState }, command);
}

void dae::InputManager::RemoveCommand(Player player, ControllerButton button, InputState inputState)
{
	m_ControllerCommandMap[player].erase(ControllerKey{ button, inputState });
}

void dae::InputManager::RemoveCommand(KeyboardCode key, InputState inputState)
{
	m_KeyboardCommandMap.erase(KeyboardKey{ key, inputState });
}

void dae::InputManager::ClearCommands()
{
	for (auto& pair : m_ControllerCommandMap)
	{
		pair.second.clear();
	}
	m_KeyboardCommandMap.clear();
}

void dae::InputManager::UpdateConnectedControllers()
{
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		dwResult = XInputGetState(i, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			m_Controllers[i].ActivateController();
		}
	}
}

void dae::InputManager::HandleInput()
{
	HandleControllerInput();
	HandleKeyboardInput();
}

void dae::InputManager::HandleControllerInput()
{
	for (auto& playerPair : m_ControllerCommandMap)
	{
		const int player = static_cast<int>(playerPair.first);
		for (auto& commandPair : playerPair.second)
		{
			switch (commandPair.first.second)
			{
			case InputState::Down:
				if (m_Controllers[player].IsDown(commandPair.first.first))
				{
					commandPair.second();
				}
				break;
			case InputState::Up:
				if (m_Controllers[player].IsUp(commandPair.first.first))
				{
					commandPair.second();
				}
				break;
			case InputState::Hold:
				if (m_Controllers[player].IsPressed(commandPair.first.first))
				{
					commandPair.second();
				}
			}
		}
	}
}

void dae::InputManager::HandleKeyboardInput()
{
	for (auto& commandPair : m_KeyboardCommandMap)
	{
		switch (commandPair.first.second)
		{
		case InputState::Down:
			if (IsDown(commandPair.first.first))
			{
				commandPair.second();
			}
			break;
		case InputState::Up:
			if (IsUp(commandPair.first.first))
			{
				commandPair.second();
			}
			break;
		case InputState::Hold:
			if (IsPressed(commandPair.first.first))
			{
				commandPair.second();
			}
		}
	}
}

void dae::InputManager::UpdateControllerStates()
{
	for (DWORD i = 0; i < m_MaxControllers; ++i)
	{
		m_Controllers[i].UpdateController(i);
	}
}

void dae::InputManager::UpdatePreviousKeyboardState()
{
	for (auto down : m_KeysDown)
	{
		m_PreviousKeyboardState[static_cast<int>(down)] = true;
	}
	m_KeysDown.clear();
	for (auto up : m_KeysUp)
	{
		m_PreviousKeyboardState[static_cast<int>(up)] = false;
	}
	m_KeysUp.clear();
}
