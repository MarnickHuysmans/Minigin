#pragma once
struct _XINPUT_STATE;

namespace dae
{
	enum class ControllerButton : int
	{
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000,
		ButtonBack = 0x0020,
		ButtonStart = 0x0010,
		DpadUp = 0x0001,
		DpadDown = 0x0002,
		DpadLeft = 0x0004,
		DpadRight = 0x0008,
		ShoulderLeft = 0x0100,
		ShoulderRight = 0x0200,
		LeftThumb = 0x0040,
		RightThumb = 0x0080
	};
	
	class Controller final
	{
	public:
		Controller();
		~Controller();

		void ActivateController();
		void UpdateController(DWORD index);

		bool IsPressed(ControllerButton button) const;
		bool IsDown(ControllerButton button) const;
		bool IsUp(ControllerButton button) const;

		Controller(const Controller& other) = delete;
		Controller(Controller&& other) noexcept = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other) noexcept = delete;
		
	private:
		_XINPUT_STATE* m_PreviousControllerState;
		_XINPUT_STATE* m_CurrentControllerState;
		bool m_Active;
	};
}
