#include "MiniginPCH.h"
#include "Qbert.h"

#include "InputManager.h"


void qbert::Qbert::Start()
{
}

void qbert::Qbert::Update()
{
	if (m_ControlsrSet)
	{
		auto& inputManager = InputManager::GetInstance();
		if (m_UseController)
		{
			if (inputManager.IsDown(m_ControllerButton[0]))
			{
				Die();
			}
			if (inputManager.IsDown(m_ControllerButton[1]))
			{
				m_Subject.Notify("Color");
			}
			if (inputManager.IsDown(m_ControllerButton[2]))
			{
				m_Subject.Notify("Coily");
			}
			if (inputManager.IsDown(m_ControllerButton[3]))
			{
				m_Subject.Notify("Discs");
			}
			if (inputManager.IsDown(m_ControllerButton[4]))
			{
				m_Subject.Notify("SlickSam");
			}
		}
		else
		{
			if (inputManager.IsDown(m_Scancode[0]))
			{
				Die();
			}
			if (inputManager.IsDown(m_Scancode[1]))
			{
				m_Subject.Notify("Color");
			}
			if (inputManager.IsDown(m_Scancode[2]))
			{
				m_Subject.Notify("Coily");
			}
			if (inputManager.IsDown(m_Scancode[3]))
			{
				m_Subject.Notify("Discs");
			}
			if (inputManager.IsDown(m_Scancode[4]))
			{
				m_Subject.Notify("SlickSam");
			}
		}
	}
}

int qbert::Qbert::GetLives() const
{
	return m_Lives;
}

void qbert::Qbert::AddObserver(Observer* observer)
{
	m_Subject.AddObserver(observer);
}

void qbert::Qbert::RemoveObserver(Observer* observer)
{
	m_Subject.RemoveObserver(observer);
}

void qbert::Qbert::SetButtons(ControllerButton button1, ControllerButton button2, ControllerButton button3, ControllerButton button4, ControllerButton button5)
{
	m_ControlsrSet = true;
	m_UseController = true;
	m_ControllerButton[0] = button1;
	m_ControllerButton[1] = button2;
	m_ControllerButton[2] = button3;
	m_ControllerButton[3] = button4;
	m_ControllerButton[4] = button5;
}

void qbert::Qbert::SetButtons(KeyboardSDL button1, KeyboardSDL button2, KeyboardSDL button3, KeyboardSDL button4, KeyboardSDL button5)
{
	m_ControlsrSet = true;
	m_UseController = false;
	m_Scancode[0] = button1;
	m_Scancode[1] = button2;
	m_Scancode[2] = button3;
	m_Scancode[3] = button4;
	m_Scancode[4] = button5;
}


void qbert::Qbert::Die()
{
	if (m_Lives > 0)
	{
		--m_Lives;
		std::cout << "Qbert dies: " << m_Lives << " left" << std::endl;
		m_Subject.Notify("PlayerDied");
	}
}
