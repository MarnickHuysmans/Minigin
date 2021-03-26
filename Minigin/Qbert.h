#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae {
	enum class ControllerButton;
	enum class KeyboardSDL;
}

namespace qbert
{
	using namespace dae;
	
	class Qbert : public Component
	{
	public:
		void Start() override;
		void Update() override;

		int GetLives() const;

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

		void SetButtons(ControllerButton button1, ControllerButton button2, ControllerButton button3, ControllerButton button4, ControllerButton button5);
		void SetButtons(KeyboardSDL button1, KeyboardSDL button2, KeyboardSDL button3, KeyboardSDL button4, KeyboardSDL button5);

	private:

		void Die();
		
		Subject m_Subject{};
		int m_Lives{10};
		bool m_ControlsrSet = false;
		bool m_UseController = true;
		ControllerButton m_ControllerButton[5]{};
		KeyboardSDL m_Scancode[5]{};
	};
}
