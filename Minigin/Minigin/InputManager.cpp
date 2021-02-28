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

void dae::InputManager::AddCommand(std::unique_ptr<Command>& command, Player player, ControllerButton button, InputState inputState)
{
	m_ControllerCommandMap[player][ControllerKey{ button, inputState }] = std::move(command);
}

void dae::InputManager::AddCommand(std::unique_ptr<Command>& command, SDL_Scancode key, InputState inputState)
{
	m_KeyboardCommandMap[KeyboardKey{ key, inputState }] = std::move(command);
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
					commandPair.second->Execute();
				}
				break;
			case InputState::Up:
				if (m_Controllers[player].IsUp(commandPair.first.first))
				{
					commandPair.second->Execute();
				}
				break;
			case InputState::Hold:
				if (m_Controllers[player].IsPressed(commandPair.first.first))
				{
					commandPair.second->Execute();
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
		m_PreviousKeyboardState[down] = true;
	}
	m_KeysDown.clear();
	for (auto up : m_KeysUp)
	{
		m_PreviousKeyboardState[up] = false;
	}
	m_KeysUp.clear();
}
