#pragma once
#include <memory>

namespace qbert
{
	class IState;

	class StateMachine final
	{
	public:
		StateMachine() = default;
		~StateMachine() = default;

		StateMachine(const StateMachine& other) = delete;
		StateMachine(StateMachine&& other) noexcept = delete;
		StateMachine& operator=(const StateMachine& other) = delete;
		StateMachine& operator=(StateMachine&& other) noexcept = delete;

		void Initialize(std::shared_ptr<IState> state);
		void Update();
		void ChangeState(std::shared_ptr<IState> newState);

	private:
		std::shared_ptr<IState> m_CurrentState;
	};
}
