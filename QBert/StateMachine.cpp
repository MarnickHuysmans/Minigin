#include "StateMachine.h"

#include <utility>
#include "IState.h"

void qbert::StateMachine::Initialize(std::shared_ptr<IState> state)
{
	m_CurrentState = std::move(state);
	m_CurrentState->Enter();
}

void qbert::StateMachine::Update()
{
	m_CurrentState->Update();
}

void qbert::StateMachine::ChangeState(std::shared_ptr<IState> newState)
{
	if (m_CurrentState == newState)
	{
		return;
	}
	m_CurrentState->Exit();

	m_CurrentState = std::move(newState);
	m_CurrentState->Enter();
}
