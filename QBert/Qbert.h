#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae {
	enum class ControllerButton;
	enum class KeyboardCode;
}

namespace qbert
{
	class Qbert final : public dae::Component
	{
	public:
		void Start() override;
		void Update() override;

		int GetLives() const;

		void AddObserver(dae::Observer* observer);
		void RemoveObserver(dae::Observer* observer);

		void SetButtons(dae::ControllerButton button1, dae::ControllerButton button2, dae::ControllerButton button3, dae::ControllerButton button4, dae::ControllerButton button5);
		void SetButtons(dae::KeyboardCode button1, dae::KeyboardCode button2, dae::KeyboardCode button3, dae::KeyboardCode button4, dae::KeyboardCode button5);
		
	private:
		void Die();

		dae::Subject m_Subject{};
		int m_Lives{ 10 };
		bool m_ControlsrSet = false;
		bool m_UseController = true;
		dae::ControllerButton m_ControllerButton[5]{};
		dae::KeyboardCode m_Scancode[5]{};
	};
}
