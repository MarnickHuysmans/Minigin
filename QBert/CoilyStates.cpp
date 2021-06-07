#include "CoilyStates.h"

#include <algorithm>

#include "Coily.h"
#include "GameObject.h"
#include "Movement.h"
#include "MoveDown.h"
#include "Walkable.h"
#include "PlayerInput.h"
#include "EnemySpawner.h"
#include "GameTime.h"
#include "Level.h"
#include "RenderComponent.h"
#include "ServiceLocator.h"

qbert::CoilyMoveDownState::CoilyMoveDownState(StateMachine& stateMachine, Coily* coily) :
	m_StateMachine(stateMachine),
	m_Coily(coily)
{
	auto weakGameObject = m_Coily->GetGameObject();
	if (weakGameObject.expired())
	{
		return;
	}
	auto gameObject = weakGameObject.lock();
	m_MoveDown = gameObject->GetComponent<MoveDown>();
	m_RenderComponent = gameObject->GetComponent<dae::RenderComponent>();
	m_Movement = gameObject->GetComponent<Movement>();
}

void qbert::CoilyMoveDownState::Initialize()
{
	if (m_Movement.expired())
	{
		return;
	}
	m_Movement.lock()->AddObserver(weak_from_this());
}

void qbert::CoilyMoveDownState::Enter()
{
	if (m_Movement.expired())
	{
		return;
	}
	m_Movement.lock()->SetMoveTime(0);
}

void qbert::CoilyMoveDownState::Update()
{
}

void qbert::CoilyMoveDownState::Exit()
{
	if (!m_MoveDown.expired())
	{
		m_MoveDown.lock()->SetActive(false);
	}
	if (!m_Movement.expired())
	{
		m_Movement.lock()->SetPositionOffset(m_Coily->GetOffset());
	}
	if (!m_RenderComponent.expired())
	{
		m_RenderComponent.lock()->SetTexture(m_Coily->GetTexture());
	}
}

void qbert::CoilyMoveDownState::Fall()
{
}

void qbert::CoilyMoveDownState::Moved(std::weak_ptr<qbert::Movement> movement)
{
	if (movement.expired())
	{
		return;
	}
	auto current = movement.lock()->GetCurrentWalkable();
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
	auto weakGameObject = m_Coily->GetGameObject();
	if (weakGameObject.expired())
	{
		return;
	}
	m_Movement = weakGameObject.lock()->GetComponent<Movement>();
}

void qbert::CoilyAIState::Initialize()
{
	if (!m_Coily->IsPlayer())
	{
		return;
	}
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

	if (m_Movement.expired())
	{
		return;
	}
	auto enemyMovement = m_Movement.lock();

	auto weakWalkable = enemyMovement->GetCurrentWalkable();
	if (weakWalkable.expired())
	{
		return;
	}
	auto enemyWalkable = weakWalkable.lock();

	auto enemySpawner = m_Coily->GetEnemySpawner();
	if (enemySpawner.expired())
	{
		return;
	}
	auto& playerMovements = enemySpawner.lock()->GetPlayerMovements();

	if (playerMovements.empty())
	{
		return;
	}

	if (playerMovements.size() == 1)
	{
		MoveTo(playerMovements[0], enemyWalkable, enemyMovement);
		return;
	}

	auto it = std::min_element(std::begin(playerMovements), std::end(playerMovements),
		[&enemyWalkable](const std::weak_ptr<Movement>& leftMovement, const std::weak_ptr<Movement>& rightMovement)
		{
			if (leftMovement.expired())
			{
				return false;
			}
			if (rightMovement.expired())
			{
				return true;
			}
			auto left = leftMovement.lock()->GetCurrentWalkable();
			if (left.expired())
			{
				return false;
			}
			auto right = rightMovement.lock()->GetCurrentWalkable();
			if (right.expired())
			{
				return true;
			}
			return enemyWalkable->GetDistanceTo(left.lock()) < enemyWalkable->GetDistanceTo(right.lock());
		});
	if (it == std::end(playerMovements) || it->expired())
	{
		return;
	}
	MoveTo(*it, enemyWalkable, enemyMovement);
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

void qbert::CoilyAIState::MoveTo(const std::weak_ptr<Movement>& playerMovement, const std::shared_ptr<Walkable>& enemyWalkable, const std::shared_ptr<Movement>& enemyMovement)
{
	if (playerMovement.expired())
	{
		return;
	}
	auto weakWalkable = playerMovement.lock()->GetCurrentWalkable();
	if (weakWalkable.expired())
	{
		return;
	}
	dae::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sound/Ahop.wav");
	auto playerWalkable = weakWalkable.lock();

	int enemyRow = enemyWalkable->GetRow();
	int enemyCol = enemyWalkable->GetColumn();
	int playerRow = playerWalkable->GetRow();
	int playerCol = playerWalkable->GetColumn();

	if (enemyCol == playerCol)
	{
		enemyMovement->Move(enemyRow < playerRow ? Direction::Down : Direction::Up);
		return;
	}

	if (enemyCol - playerCol == enemyRow - playerRow)
	{
		enemyMovement->Move(enemyRow < playerRow ? Direction::Right : Direction::Left);
		return;
	}

	if (enemyRow >= playerRow)
	{
		enemyMovement->Move(enemyCol < playerCol ? Direction::Up : Direction::Left);
		return;
	}

	enemyMovement->Move(enemyCol < playerCol ? Direction::Right : Direction::Down);
}

qbert::CoilyPlayerState::CoilyPlayerState(StateMachine& stateMachine, Coily* coily) :
	m_StateMachine(stateMachine),
	m_Coily(coily)
{
	if (!m_Coily->IsPlayer())
	{
		return;
	}

	auto weakGameObject = m_Coily->GetGameObject();
	if (weakGameObject.expired())
	{
		return;
	}
	auto gameObject = weakGameObject.lock();
	m_Movement = gameObject->GetComponent<Movement>();
	m_PlayerInput = gameObject->GetComponent<PlayerInput>();

	if (m_PlayerInput.expired())
	{
		return;
	}
	m_PlayerInput.lock()->SetActive(false);
}

void qbert::CoilyPlayerState::Initialize()
{
	if (!m_Coily->IsPlayer())
	{
		return;
	}
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
	m_Movement.lock()->ResetMoveTime();
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

void qbert::CoilyPlayerState::Moved(std::weak_ptr<Movement> movement)
{
	dae::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sound/Ahop.wav");
	if (movement.expired())
	{
		return;
	}
	auto current = movement.lock()->GetCurrentWalkable();
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
