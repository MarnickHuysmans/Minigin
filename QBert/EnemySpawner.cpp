#include "EnemySpawner.h"

#include "EnemyFactory.h"
#include "EnemyHit.h"
#include "Level.h"
#include "GameObject.h"
#include "GameTime.h"
#include "MathHelper.h"

qbert::EnemySpawner::EnemySpawner(const std::weak_ptr<Level>& level, const std::vector<std::weak_ptr<Movement>>& playerMovements, bool coilyPlayer, float slickSamMinTime, float slickSamMaxTime, float uggWrongWayMinTime, float uggWrongWayMaxTime, float coilyMinTime, float coilyMaxTime) :
	m_PlayerMovements(playerMovements),
	m_Level(level),
	m_SlickSamTimer(slickSamMaxTime),
	m_SlickSamTimeMin(slickSamMinTime),
	m_SlickSamTimeMax(slickSamMaxTime),
	m_UggWrongWayTimer(uggWrongWayMaxTime),
	m_UggWrongWayTimeMin(uggWrongWayMinTime),
	m_UggWrongWayTimeMax(uggWrongWayMaxTime),
	m_CoilyTimer(coilyMaxTime),
	m_CoilyTimeMin(coilyMinTime),
	m_CoilyTimeMax(coilyMaxTime),
	m_QbertSpawnDelay(),
	m_EraseTimer(30),
	m_EraseTime(m_EraseTimer),
	m_CoilyPlayer(coilyPlayer)
{
}

void qbert::EnemySpawner::Update()
{
	float elapsed = dae::GameTime::GetInstance().GetDeltaTime();
	SpawnSlickSam(elapsed);
	SpawnUggWrongWay(elapsed);
	SpawnCoily(elapsed);
	EraseEnemies(elapsed);
}

const std::vector<std::weak_ptr<qbert::Movement>>& qbert::EnemySpawner::GetPlayerMovements() const
{
	return m_PlayerMovements;
}

void qbert::EnemySpawner::AddScore(unsigned score)
{
	if (m_Level.expired())
	{
		return;
	}
	m_Level.lock()->AddScore(score);
}

void qbert::EnemySpawner::LevelDisc()
{
	DestroyEnemies();
	m_SlickSamTimer = m_SlickSamTimeMax;
	m_UggWrongWayTimer = randF(m_UggWrongWayTimeMin, m_UggWrongWayTimeMax);
}

void qbert::EnemySpawner::NextLevel()
{
	DestroyEnemies();
	DestroyCoily();
	m_SlickSamTimer = m_SlickSamTimeMax;
	m_UggWrongWayTimer = m_UggWrongWayTimeMax;
	m_CoilyTimer = m_CoilyTimeMax;
}

void qbert::EnemySpawner::GameComplete()
{
	DestroyEnemies();
	DestroyCoily();
}

void qbert::EnemySpawner::QbertLives(int lives)
{
	m_UggWrongWayTimer = m_UggWrongWayTimeMax;
}

void qbert::EnemySpawner::QbertRespawn()
{
}

void qbert::EnemySpawner::Fall()
{
}

void qbert::EnemySpawner::Moved(Movement* movement)
{
	m_Enemies.erase(std::remove_if(std::begin(m_Enemies), std::end(m_Enemies), [&movement](const std::weak_ptr<dae::GameObject>& enemy)
		{
			if (enemy.expired())
			{
				return true;
			}
			auto enemyHit = enemy.lock()->GetComponent<EnemyHit>();
			if (enemyHit.expired())
			{
				return false;
			}
			enemyHit.lock()->PlayerMoved(movement);
			return false;
		}), std::end(m_Enemies));
	m_EraseTimer = m_EraseTime;

	if (m_Coily.expired())
	{
		return;
	}
	auto enemyHit = m_Coily.lock()->GetComponent<EnemyHit>();
	if (enemyHit.expired())
	{
		return;
	}
	enemyHit.lock()->PlayerMoved(movement);
}

void qbert::EnemySpawner::CoilyFall()
{
	m_CoilyTimer = randF(m_CoilyTimeMin, m_CoilyTimeMax);
}

void qbert::EnemySpawner::SpawnSlickSam(float elapsed)
{
	m_SlickSamTimer -= elapsed;
	if (m_SlickSamTimer > 0)
	{
		return;
	}
	m_SlickSamTimer += randF(m_SlickSamTimeMin, m_SlickSamTimeMax);
	bool slick = rand() % 2;
	if (slick)
	{
		m_Enemies.push_back(EnemyFactory::CreateSlick(this->shared_from_this()));
	}
	else
	{
		m_Enemies.push_back(EnemyFactory::CreateSam(this->shared_from_this()));
	}
}

void qbert::EnemySpawner::SpawnUggWrongWay(float elapsed)
{
	m_UggWrongWayTimer -= elapsed;
	if (m_UggWrongWayTimer > 0)
	{
		return;
	}
	m_UggWrongWayTimer += randF(m_UggWrongWayTimeMin, m_UggWrongWayTimeMax);
	bool ugg = rand() % 2;
	if (ugg)
	{
		m_Enemies.push_back(EnemyFactory::CreateUgg(this->shared_from_this()));
	}
	else
	{
		m_Enemies.push_back(EnemyFactory::CreateWrongWay(this->shared_from_this()));
	}
}

void qbert::EnemySpawner::SpawnCoily(float elapsed)
{
	if (m_CoilyTimer <= 0)
	{
		return;
	}
	m_CoilyTimer -= elapsed;
	if (m_CoilyTimer > 0)
	{
		return;
	}
	m_Coily = EnemyFactory::CreateCoily(this->shared_from_this(), m_CoilyPlayer);
}

void qbert::EnemySpawner::EraseEnemies(float elapsed)
{
	m_EraseTimer -= elapsed;
	if (m_EraseTimer > 0)
	{
		return;
	}
	m_EraseTimer = m_EraseTime;
	m_Enemies.erase(std::remove_if(std::begin(m_Enemies), std::end(m_Enemies), [](const std::weak_ptr<dae::GameObject>& enemy)
	{
		return enemy.expired();
	}), std::end(m_Enemies));
}

void qbert::EnemySpawner::DestroyEnemies()
{
	for (auto& enemy : m_Enemies)
	{
		if (enemy.expired())
		{
			continue;
		}
		enemy.lock()->Destroy();
	}
	m_Enemies.clear();
}

void qbert::EnemySpawner::DestroyCoily()
{
	if (m_Coily.expired())
	{
		return;
	}
	m_Coily.lock()->Destroy();
}
