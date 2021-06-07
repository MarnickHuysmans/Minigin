#include "EnemyHit.h"
#include "Qbert.h"
#include "EnemySpawner.h"
#include "Movement.h"
#include "GameObject.h"
#include "Walkable.h"

qbert::EnemyHit::EnemyHit(const std::weak_ptr<EnemySpawner>& enemySpawner, Side side) :
	m_EnemySpawner(enemySpawner),
	m_Side(side)
{
}

void qbert::EnemyHit::Fall()
{
	if (m_GameObject.expired())
	{
		return;
	}
	m_GameObject.lock()->Destroy();
}

void qbert::EnemyHit::Moved(std::weak_ptr<qbert::Movement> movement)
{
	if (!ActiveInScene() || m_EnemySpawner.expired() || movement.expired())
	{
		return;
	}

	m_CurrentLocation = movement.lock()->GetCurrentWalkable();
	if (m_CurrentLocation.expired())
	{
		return;
	}

	auto& players = m_EnemySpawner.lock()->GetPlayerMovements();
	for (auto& player : players)
	{
		if (player.expired())
		{
			continue;
		}
		CheckPlayerMovement(player.lock().get());
	}
}

void qbert::EnemyHit::PlayerMoved(const Movement* playerMovement)
{
	if (!ActiveInScene())
	{
		return;
	}

	if (m_CurrentLocation.expired())
	{
		return;
	}
	CheckPlayerMovement(playerMovement);
}

void qbert::EnemyHit::OnPlayerHit(Qbert* qbert)
{
	qbert->Damage();
}

void qbert::EnemyHit::CheckPlayerMovement(const Movement* playerMovement)
{
	auto playerWalkable = playerMovement->GetCurrentWalkable();
	if (playerWalkable.expired())
	{
		return;
	}

	if (HitCheck(playerWalkable.lock().get(), m_CurrentLocation.lock().get()))
	{
		auto weakGameObject = playerMovement->GetGameObject();
		if (weakGameObject.expired())
		{
			return;
		}
		auto qbert = weakGameObject.lock()->GetComponent<Qbert>();
		if (qbert.expired())
		{
			return;
		}
		OnPlayerHit(qbert.lock().get());
	}
}

bool qbert::EnemyHit::HitCheck(Walkable* playerLocation, Walkable* location) const
{
	switch (m_Side)
	{
	case Side::Top:
		return playerLocation == location;
	case Side::Right:
		return playerLocation->GetRow() - 1 == location->GetRow() && playerLocation->GetColumn() - 1 == location->GetColumn();
	case Side::Left:
		return playerLocation->GetRow() - 1 == location->GetRow() && playerLocation->GetColumn() == location->GetColumn();
	default:
		return false;
	}
}
