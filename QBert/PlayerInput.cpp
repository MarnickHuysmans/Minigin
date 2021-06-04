#include "PlayerInput.h"
#include "GameObject.h"
#include "Movement.h"

qbert::InputType qbert::PlayerInput::m_Player1 = InputType::Keyboard;
qbert::InputType qbert::PlayerInput::m_Player2 = InputType::Keyboard;

qbert::PlayerInput::PlayerInput(dae::Player player, bool keyboard) :
	m_Player(player),
	m_Keyboard(keyboard),
	m_Enabled(true)
{
}

qbert::PlayerInput::~PlayerInput()
{
	auto& inputManager = dae::InputManager::GetInstance();
	if (m_Keyboard)
	{
		RemoveKeyboardInput(inputManager);
	}
	else
	{
		RemoveControllerInput(inputManager);
	}
}

void qbert::PlayerInput::Start()
{
	m_Movement = m_GameObject->GetComponent<Movement>();
	if (m_Movement.expired())
	{
		return;
	}
	auto& inputManager = dae::InputManager::GetInstance();
	if (m_Keyboard)
	{
		KeyboardInput(inputManager);
	}
	else
	{
		ControllerInput(inputManager);
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
			if (m_Movement.expired() || !m_Enabled)
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Up);
		}, m_Player == dae::Player::Player1 ? dae::KeyboardCode::SDL_SCANCODE_W : dae::KeyboardCode::SDL_SCANCODE_UP, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !m_Enabled)
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Right);
		}, m_Player == dae::Player::Player1 ? dae::KeyboardCode::SDL_SCANCODE_D : dae::KeyboardCode::SDL_SCANCODE_RIGHT, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !m_Enabled)
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Down);
		}, m_Player == dae::Player::Player1 ? dae::KeyboardCode::SDL_SCANCODE_S : dae::KeyboardCode::SDL_SCANCODE_DOWN, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !m_Enabled)
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
			if (m_Movement.expired() || !m_Enabled)
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Up);
		}, m_Player, dae::ControllerButton::DpadUp, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !m_Enabled)
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Right);
		}, m_Player, dae::ControllerButton::DpadRight, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !m_Enabled)
			{
				return;
			}
			m_Movement.lock()->Move(Direction::Down);
		}, m_Player, dae::ControllerButton::DpadDown, dae::InputState::Hold);

	inputManager.AddCommand([this]()
		{
			if (m_Movement.expired() || !m_Enabled)
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
