#include "Coily.h"

#include <utility>
#include "EnemySpawner.h"
#include "GameObject.h"
#include "ServiceLocator.h"

qbert::Coily::Coily(std::weak_ptr<EnemySpawner> enemySpawner, bool player, const glm::vec3& offset,
                    std::shared_ptr<dae::Texture2D> texture, float moveTime) :
	m_Offset(offset),
	m_Texture(std::move(texture)),
	m_EnemySpawner(std::move(enemySpawner)),
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
	return m_PlayerState;
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

const glm::vec3& qbert::Coily::GetOffset() const
{
	return m_Offset;
}

const std::shared_ptr<dae::Texture2D>& qbert::Coily::GetTexture() const
{
	return m_Texture;
}

void qbert::Coily::Fall()
{
	if (m_EnemySpawner.expired())
	{
		return;
	}
	m_EnemySpawner.lock()->CoilyFall();
	if (m_GameObject.expired())
	{
		return;
	}
	m_GameObject.lock()->Destroy();
	dae::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sound/CoilyOverTheEdge.wav");
}

void qbert::Coily::Moved(std::weak_ptr<Movement>)
{
}
