#include "CoilyStates.h"
#include "Coily.h"
#include "GameObject.h"
#include "Movement.h"
#include "MoveDown.h"
#include "Walkable.h"
#include "PlayerInput.h"
#include "EnemySpawner.h"
#include "GameTime.h"
#include "Level.h"

qbert::CoilyMoveDownState::CoilyMoveDownState(StateMachine& stateMachine, Coily* coily) :
	m_StateMachine(stateMachine),
	m_Coily(coily)
{
	m_MoveDown = m_Coily->GetGameObject()->GetComponent<MoveDown>();
}

void qbert::CoilyMoveDownState::Initialize()
{
	auto movement = m_Coily->GetGameObject()->GetComponent<Movement>();
	if (movement.expired())
	{
		return;
	}
	movement.lock()->AddObserver(weak_from_this());
}

void qbert::CoilyMoveDownState::Enter()
{
}

void qbert::CoilyMoveDownState::Update()
{
}

void qbert::CoilyMoveDownState::Exit()
{
	if (m_MoveDown.expired())
	{
		return;
	}
	m_MoveDown.lock()->SetActive(false);
}

void qbert::CoilyMoveDownState::Fall()
{
}

void qbert::CoilyMoveDownState::Moved(Movement* movement)
{
	auto current = movement->GetCurrentWalkable();
	if (current.expired())
	{
		return;
	}
	auto next = current.lock()->GetWalkable(Side::Top, Direction::Down);
	if (next.expired())
	{
		std::shared_ptr<IState> nextState;
		if (m_Coily->IsPlayer())
		{
			nextState = m_Coily->GetPlayerState();
		}
		else
		{
			nextState = m_Coily->GetAiState();
		}
		m_StateMachine.ChangeState(nextState);
	}
}

qbert::CoilyAIState::CoilyAIState(StateMachine& stateMachine, Coily* coily) :
	m_StateMachine(stateMachine),
	m_Coily(coily),
	m_MoveTimer(m_Coily->GetMoveTime())
{
	m_Movement = m_Coily->GetGameObject()->GetComponent<Movement>();
}

void qbert::CoilyAIState::Initialize()
{
	auto enemySpawner = m_Coily->GetEnemySpawner();
	if (enemySpawner.expired())
	{
		return;
	}
	auto level = enemySpawner.lock()->GetLevel();
	if (level.expired())
	{
		return;
	}
	level.lock()->AddObserver(weak_from_this());
}

void qbert::CoilyAIState::Enter()
{
}

void qbert::CoilyAIState::Update()
{
	m_MoveTimer -= dae::GameTime::GetInstance().GetDeltaTime();
	if (m_MoveTimer > 0)
	{
		return;
	}
	m_MoveTimer += m_Coily->GetMoveTime();
	auto enemySpawner = m_Coily->GetEnemySpawner();
	if (enemySpawner.expired())
	{
		return;
	}
	auto& playerMovements = enemySpawner.lock()->GetPlayerMovements();
	//TODO Finish AiState
}

void qbert::CoilyAIState::Exit()
{
	m_MoveTimer = m_Coily->GetMoveTime();
}

void qbert::CoilyAIState::LevelDisc()
{
	if (!m_Coily->IsPlayer())
	{
		return;
	}
	m_StateMachine.ChangeState(m_Coily->GetPlayerState());
}

void qbert::CoilyAIState::NextLevel()
{
}

void qbert::CoilyAIState::GameComplete()
{
}

qbert::CoilyPlayerState::CoilyPlayerState(StateMachine& stateMachine, Coily* coily) :
	m_StateMachine(stateMachine),
	m_Coily(coily)
{
	if (!m_Coily->IsPlayer())
	{
		return;
	}
	m_Movement = m_Coily->GetGameObject()->GetComponent<Movement>();
	m_PlayerInput = m_Coily->GetGameObject()->GetComponent<PlayerInput>();

	if (m_PlayerInput.expired())
	{
		return;
	}
	m_PlayerInput.lock()->SetActive(false);
}

void qbert::CoilyPlayerState::Initialize()
{
	RegisterMovement();
}

void qbert::CoilyPlayerState::Enter()
{
	if (m_PlayerInput.expired())
	{
		m_StateMachine.ChangeState(m_Coily->GetAiState());
		return;
	}
	m_PlayerInput.lock()->SetActive(true);
	if (m_Movement.expired())
	{
		return;
	}
	m_Movement.lock()->SetMoveTime();
}

void qbert::CoilyPlayerState::Update()
{
}

void qbert::CoilyPlayerState::Exit()
{
	if (!m_Movement.expired())
	{
		m_Movement.lock()->SetMoveTime(0);
	}
	if (m_PlayerInput.expired())
	{
		return;
	}
	m_PlayerInput.lock()->SetActive(false);
}

void qbert::CoilyPlayerState::Fall()
{
}

void qbert::CoilyPlayerState::Moved(Movement* movement)
{
	auto current = movement->GetCurrentWalkable();
	if (current.expired())
	{
		return;
	}
	if (current.lock()->GetWalkableType() == Walkable::WalkableType::Disc)
	{
		m_StateMachine.ChangeState(m_Coily->GetAiState());
	}
}

void qbert::CoilyPlayerState::RegisterMovement()
{
	auto enemySpawner = m_Coily->GetEnemySpawner();
	if (enemySpawner.expired())
	{
		return;
	}
	auto& playerMovements = enemySpawner.lock()->GetPlayerMovements();
	for (auto playerMovement : playerMovements)
	{
		if (playerMovement.expired())
		{
			continue;
		}
		playerMovement.lock()->AddObserver(weak_from_this());
	}
}
