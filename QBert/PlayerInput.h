#pragma once
#include <Component.h>

#include "InputManager.h"

namespace qbert
{
	class Movement;

	enum class InputType : int
	{
		Keyboard = 0,
		Controller = 1
	};

	class PlayerInput : public dae::Component
	{
	public:
		PlayerInput(dae::Player player);
		virtual ~PlayerInput();

		void Start() override;

		static void SetPlayer1(InputType input);
		static void SetPlayer2(InputType input);
		static InputType GetPlayer1();
		static InputType GetPlayer2();

	private:
		void KeyboardInput(dae::InputManager& inputManager);
		void ControllerInput(dae::InputManager& inputManager);
		void RemoveKeyboardInput(dae::InputManager& inputManager) const;
		void RemoveControllerInput(dae::InputManager& inputManager) const;

		std::weak_ptr<Movement> m_Movement;
		dae::Player m_Player;
		InputType m_InputType;

		static InputType m_Player1;
		static InputType m_Player2;
	};
}
