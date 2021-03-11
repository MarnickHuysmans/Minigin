#include "MiniginPCH.h"
#include "Subject.h"
#include "Observer.h"

void dae::Subject::AddObserver(Observer* observer)
{
	m_Observers.push_back(observer);
}

void dae::Subject::RemoveObserver(Observer* observer)
{
	m_Observers.erase(std::remove(std::begin(m_Observers), std::end(m_Observers), observer), std::end(m_Observers));
}

void dae::Subject::Notify(const std::string& message)
{
	for (auto observer : m_Observers)
	{
		observer->Notify(message);
	}
}
