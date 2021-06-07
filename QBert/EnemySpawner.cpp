#include "EnemySpawner.h"

#include <utility>

#include "EnemyFactory.h"
#include "EnemyHit.h"
#include "Level.h"
#include "GameObject.h"
#include "GameTime.h"
#include "MathHelper.h"
#include "Movement.h"

unsigned int qbert::EnemySpawner::m_CoilyScore = 500;

qbert::EnemySpawner::EnemySpawner(std::weak_ptr<Level> level,
	std::vector<std::weak_ptr<Movement>> playerMovements, bool coilyPlayer,
	float slickSamMinTime, float slickSamMaxTime, float uggWrongWayMinTime,
	float uggWrongWayMaxTime, float coilyMinTime, float coilyMaxTime) :
	m_PlayerMovements(std::move(playerMovements)),
	m_Level(std::move(level)),
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
	m_CoilyPlayer(coilyPlayer),
	m_DestroyEnemies(false),
	m_DestroyCoily(false),
	m_Spawning(true),
	m_CoilyFell(false)
{
}

void qbert::EnemySpawner::Update()
{
	if (m_DestroyEnemies)
	{
		DestroyEnemies();
	}
	if (m_DestroyCoily)
	{
		DestroyCoily();
	}
	if (!m_Spawning)
	{
		return;
	}
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
	m_DestroyEnemies = true;
	m_SlickSamTimer = m_SlickSamTimeMax;
	m_UggWrongWayTimer = randF(m_UggWrongWayTimeMin, m_UggWrongWayTimeMax);
	if (m_CoilyFell)
	{
		m_CoilyTimer = randF(m_CoilyTimeMin, m_CoilyTimeMax);
		m_CoilyFell = false;
	}
}

void qbert::EnemySpawner::NextLevel()
{
	m_DestroyEnemies = true;
	m_DestroyCoily = true;
	m_SlickSamTimer = m_SlickSamTimeMax;
	m_UggWrongWayTimer = m_UggWrongWayTimeMax;
	m_CoilyTimer = m_CoilyTimeMax;
}

void qbert::EnemySpawner::GameComplete()
{
	m_DestroyEnemies = true;
	m_DestroyCoily = true;
	SetActive(false);
}

void qbert::EnemySpawner::QbertLives(int)
{
	m_DestroyEnemies = true;
	m_DestroyCoily = true;
	float spawnTime = randF(m_UggWrongWayTimeMin, m_UggWrongWayTimeMax);
	if (m_UggWrongWayTimer < spawnTime)
	{
		m_UggWrongWayTimer = spawnTime;
	}
	spawnTime = randF(m_SlickSamTimeMin, m_SlickSamTimeMax);
	if (m_SlickSamTimer < spawnTime)
	{
		m_SlickSamTimer = spawnTime;
	}
	m_CoilyTimer = randF(m_CoilyTimeMin, m_CoilyTimeMax);
	m_Spawning = false;
}

void qbert::EnemySpawner::QbertRespawn()
{
	m_Spawning = true;
}

void qbert::EnemySpawner::Fall()
{
}

void qbert::EnemySpawner::Moved(std::weak_ptr<Movement> movement)
{
	if (movement.expired())
	{
		return;
	}
	Movement* movementPtr = movement.lock().get();
	for (auto& coily : m_Coily)
	{
		if (coily.expired())
		{
			continue;
		}
		auto enemyHit = coily.lock()->GetComponent<EnemyHit>();
		if (enemyHit.expired())
		{
			continue;
		}
		enemyHit.lock()->PlayerMoved(movementPtr);
	}

	m_EraseTimer = m_EraseTime;
	if (m_Enemies.empty())
	{
		return;
	}
	m_Enemies.erase(std::remove_if(std::begin(m_Enemies), std::end(m_Enemies),
		[&movementPtr](const std::weak_ptr<dae::GameObject>& enemy)
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
			enemyHit.lock()->PlayerMoved(movementPtr);
			return false;
		}), std::end(m_Enemies));
}

void qbert::EnemySpawner::CoilyFall()
{
	AddScore(m_CoilyScore);
	m_CoilyFell = true;
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
		m_Enemies.push_back(EnemyFactory::CreateSlick(shared_from_this()));
	}
	else
	{
		m_Enemies.push_back(EnemyFactory::CreateSam(shared_from_this()));
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
		m_Enemies.push_back(EnemyFactory::CreateUgg(shared_from_this()));
	}
	else
	{
		m_Enemies.push_back(EnemyFactory::CreateWrongWay(shared_from_this()));
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
	DestroyCoily();
	m_Coily.push_back(EnemyFactory::CreateCoily(shared_from_this(), m_CoilyPlayer));
}

void qbert::EnemySpawner::EraseEnemies(float elapsed)
{
	m_EraseTimer -= elapsed;
	if (m_EraseTimer > 0)
	{
		return;
	}
	m_EraseTimer = m_EraseTime;
	if (m_Enemies.empty())
	{
		return;
	}
	m_Enemies.erase(std::remove_if(std::begin(m_Enemies), std::end(m_Enemies),
		[](const std::weak_ptr<dae::GameObject>& enemy)
		{
			return enemy.expired();
		}), std::end(m_Enemies));
}

void qbert::EnemySpawner::DestroyEnemies()
{
	m_DestroyEnemies = false;
	if (m_Enemies.empty())
	{
		return;
	}
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
	m_DestroyCoily = false;
	if (m_Coily.empty())
	{
		return;
	}
	for (auto& coily : m_Coily)
	{
		if (coily.expired())
		{
			continue;
		}
		coily.lock()->Destroy();
	}
	m_Coily.clear();
}
