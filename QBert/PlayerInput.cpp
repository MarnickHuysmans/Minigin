#include "PlayerInput.h"
#include "GameObject.h"
#include "Movement.h"

qbert::InputType qbert::PlayerInput::m_Player1 = InputType::Keyboard;
qbert::InputType qbert::PlayerInput::m_Player2 = InputType::Keyboard;

qbert::PlayerInput::PlayerInput(dae::Player player) :
	m_Player(player)
{
	switch (m_Player)
	{
	case dae::Player::Player1:
		m_InputType = m_Player1;
		break;
	case dae::Player::Player2:
		if (m_Player1 != m_Player2)
		{
			m_Player = dae::Player::Player1;
		}
		m_InputType = m_Player2;
		break;
	default:
		m_InputType = m_Player2;
	}
}

qbert::PlayerInput::~PlayerInput()
{
	auto& inputManager = dae::InputManager::GetInstance();
	switch (m_InputType)
	{
	case InputType::Keyboard:
		RemoveKeyboardInput(inputManager);
		break;
	case InputType::Controller:
		RemoveControllerInput(inputManager);
		break;
	}
}

void qbert::PlayerInput::Start()
{
	if (m_GameObject.expired())
	{
		return;
	}
	m_Movement = m_GameObject.lock()->GetComponent<Movement>();
	if (m_Movement.expired())
	{
		return;
	}
	auto& inputManager = dae::InputManager::GetInstance();
	switch (m_InputType)
	{
	case InputType::Keyboard:
		KeyboardInput(inputManager);
		break;
	case InputType::Controller:
		ControllerInput(inputManager);
		break;
	}
}

void qbert::PlayerInput::SetPlayer1(InputType input)
{
	m_Player1 = input;
}

void qbert::PlayerInput::SetPlayer2(InputType input)
{
	m_Player2 = input;
}

qbert::InputType qbert::PlayerInput::GetPlayer1()
{
	return m_Player1;
}

qbert::InputType qbert::PlayerInput::GetPlayer2()
{
	return m_Player2;
}

void qbert::PlayerInput::KeyboardInput(dae::InputManager& inputManager)
{
	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !ActiveInScene())
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Up);
		}, m_Player == dae::Player::Player1 ? dae::KeyboardCode::SDL_SCANCODE_W : dae::KeyboardCode::SDL_SCANCODE_UP, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !ActiveInScene())
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Right);
		}, m_Player == dae::Player::Player1 ? dae::KeyboardCode::SDL_SCANCODE_D : dae::KeyboardCode::SDL_SCANCODE_RIGHT, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !ActiveInScene())
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Down);
		}, m_Player == dae::Player::Player1 ? dae::KeyboardCode::SDL_SCANCODE_S : dae::KeyboardCode::SDL_SCANCODE_DOWN, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !ActiveInScene())
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Left);
		}, m_Player == dae::Player::Player1 ? dae::KeyboardCode::SDL_SCANCODE_A : dae::KeyboardCode::SDL_SCANCODE_LEFT, dae::InputState::Hold);
}

void qbert::PlayerInput::ControllerInput(dae::InputManager& inputManager)
{
	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !ActiveInScene())
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Up);
		}, m_Player, dae::ControllerButton::DpadUp, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !ActiveInScene())
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Right);
		}, m_Player, dae::ControllerButton::DpadRight, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !ActiveInScene())
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Down);
		}, m_Player, dae::ControllerButton::DpadDown, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !ActiveInScene())
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Left);
		}, m_Player, dae::ControllerButton::DpadLeft, dae::InputState::Hold);
}

void qbert::PlayerInput::RemoveKeyboardInput(dae::InputManager& inputManager) const
{
	inputManager.RemoveCommand(m_Player == dae::Player::Player1 ? dae::KeyboardCode::SDL_SCANCODE_W : dae::KeyboardCode::SDL_SCANCODE_UP, dae::InputState::Hold);
	inputManager.RemoveCommand(m_Player == dae::Player::Player1 ? dae::KeyboardCode::SDL_SCANCODE_D : dae::KeyboardCode::SDL_SCANCODE_RIGHT, dae::InputState::Hold);
	inputManager.RemoveCommand(m_Player == dae::Player::Player1 ? dae::KeyboardCode::SDL_SCANCODE_S : dae::KeyboardCode::SDL_SCANCODE_DOWN, dae::InputState::Hold);
	inputManager.RemoveCommand(m_Player == dae::Player::Player1 ? dae::KeyboardCode::SDL_SCANCODE_A : dae::KeyboardCode::SDL_SCANCODE_LEFT, dae::InputState::Hold);
}

void qbert::PlayerInput::RemoveControllerInput(dae::InputManager& inputManager) const
{
	inputManager.RemoveCommand(m_Player, dae::ControllerButton::DpadUp, dae::InputState::Hold);
	inputManager.RemoveCommand(m_Player, dae::ControllerButton::DpadRight, dae::InputState::Hold);
	inputManager.RemoveCommand(m_Player, dae::ControllerButton::DpadDown, dae::InputState::Hold);
	inputManager.RemoveCommand(m_Player, dae::ControllerButton::DpadLeft, dae::InputState::Hold);
}
