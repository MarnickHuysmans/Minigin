#pragma once
#include "Command.h"
#include "Component.h"
#include "Subject.h"

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

	private:
		class DieCommand : public Command
		{
		public:
			explicit DieCommand(Qbert& qbert)
				: m_Qbert(qbert)
			{
			}

			void Execute() override;
		private:
			Qbert& m_Qbert;
		};

		void Die();
		
		Subject m_Subject{};
		int m_Lives{10};
	};
}
