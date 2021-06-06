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
	m_GameObject->Destroy();
	if (m_EnemySpawner.expired())
	{
		return;
	}
	m_EnemySpawner.lock()->AddScore(m_KillScore);
}
