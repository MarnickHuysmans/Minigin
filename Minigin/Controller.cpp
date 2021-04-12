#include "MiniginPCH.h"
#include "Controller.h"
#include <XInput.h>

dae::Controller::Controller()
	:m_PreviousControllerState(new _XINPUT_STATE)
	, m_CurrentControllerState(new _XINPUT_STATE())
	, m_Active(false)
{
}

dae::Controller::~Controller()
{
	delete m_PreviousControllerState;
	delete m_CurrentControllerState;
}

void dae::Controller::ActivateController()
{
	m_Active = true;
}

void dae::Controller::UpdateController(unsigned long index)
{
	if (!m_Active)
	{
		return;
	}
	CopyMemory(m_PreviousControllerState, m_CurrentControllerState, sizeof(XINPUT_STATE));
	ZeroMemory(m_CurrentControllerState, sizeof(XINPUT_STATE));
	DWORD dwResult = XInputGetState(index, m_CurrentControllerState);
	if (dwResult != ERROR_SUCCESS)
	{
		m_Active = false;
	}
}

bool dae::Controller::IsPressed(ControllerButton button) const
{
	return m_CurrentControllerState->Gamepad.wButtons & static_cast<int>(button);
}

bool dae::Controller::IsDown(ControllerButton button) const
{
	return !(m_PreviousControllerState->Gamepad.wButtons & static_cast<int>(button)) && m_CurrentControllerState->Gamepad.wButtons & static_cast<int>(button);
}

bool dae::Controller::IsUp(ControllerButton button) const
{
	return m_PreviousControllerState->Gamepad.wButtons & static_cast<int>(button) && !(m_CurrentControllerState->Gamepad.wButtons & static_cast<int>(button));
}
