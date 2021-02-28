#include "MiniginPCH.h"
//#include "../Minigin/MiniginPCH.h"
#include "Qbert.h"

#include "InputManager.h"


void qbert::Qbert::Start()
{
	std::unique_ptr<Command> command = std::make_unique<DieCommand>(*this);
	InputManager::GetInstance().AddCommand(command, Player::Player1, ControllerButton::ButtonA);
}

void qbert::Qbert::Update()
{

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

void qbert::Qbert::DieCommand::Execute()
{
	m_Qbert.Die();
}

void qbert::Qbert::Die()
{
	if (m_Lives > 0)
	{
		--m_Lives;
		std::cout << "Qbert dies" << std::endl;
		m_Subject.Notify("PlayerDied");
	}
}
