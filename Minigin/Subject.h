#pragma once
#include <string>
#include <vector>

namespace dae
{
	class Observer;

	class Subject
	{
	public:
		void AddObserver(Observer* observer);

		void RemoveObserver(Observer* observer);

		void Notify(const std::string& message);

	private:
		std::vector<Observer*> m_Observers;
	};
}
