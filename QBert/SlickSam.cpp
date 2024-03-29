#include "SlickSam.h"
#include "GameObject.h"
#include "EnemySpawner.h"

unsigned int qbert::SlickSam::m_KillScore = 300;

qbert::SlickSam::SlickSam(const std::weak_ptr<EnemySpawner>& enemySpawner) :
	EnemyHit(enemySpawner)
{
}

void qbert::SlickSam::OnPlayerHit(Qbert*)
{
	if (m_GameObject.expired())
	{
		return;
	}
	m_GameObject.lock()->Destroy();
	if (m_EnemySpawner.expired())
	{
		return;
	}
	m_EnemySpawner.lock()->AddScore(m_KillScore);
}
