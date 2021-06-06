#include "Coily.h"

qbert::Coily::Coily(const std::weak_ptr<EnemySpawner>& enemySpawner, bool player, float moveTime) :
	m_EnemySpawner(enemySpawner),
	m_MoveTime(moveTime),
	m_IsPlayer(player)
{
}

void qbert::Coily::Start()
{
	auto moveDownState = std::make_shared<CoilyMoveDownState>(m_StateMachine, this);
	m_AiState = std::make_shared<CoilyAIState>(m_StateMachine, this);
	m_PlayerState = std::make_shared<CoilyPlayerState>(m_StateMachine, this);
	moveDownState->Initialize();
	m_AiState->Initialize();
	m_PlayerState->Initialize();
	m_StateMachine.Initialize(moveDownState);
}

void qbert::Coily::Update()
{
	m_StateMachine.Update();
}

std::shared_ptr<qbert::CoilyAIState> qbert::Coily::GetAiState() const
{
	return m_AiState;
}

std::shared_ptr<qbert::CoilyPlayerState> qbert::Coily::GetPlayerState() const
{
	return  m_PlayerState;
}

std::weak_ptr<qbert::EnemySpawner> qbert::Coily::GetEnemySpawner() const
{
	return m_EnemySpawner;
}

float qbert::Coily::GetMoveTime() const
{
	return m_MoveTime;
}

bool qbert::Coily::IsPlayer() const
{
	return m_IsPlayer;
}
