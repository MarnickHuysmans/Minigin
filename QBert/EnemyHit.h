#pragma once
#include <Component.h>

#include "MovementObserver.h"
#include "LevelEnums.h"

namespace qbert
{
	class Walkable;
	class Qbert;
	class Movement;
	class EnemySpawner;
	
	class EnemyHit : public dae::Component, public MovementObserver
	{
	public:
		EnemyHit(const std::weak_ptr<EnemySpawner>& enemySpawner, Side side = Side::Top);
		virtual ~EnemyHit() = default;

		void Fall() override final;
		void Moved(std::weak_ptr<Movement> movement) override final;

		void PlayerMoved(const Movement* playerMovement);

	protected:
		virtual void OnPlayerHit(Qbert* qbert);
		std::weak_ptr<EnemySpawner> m_EnemySpawner;
		
	private:
		void CheckPlayerMovement(const Movement* playerMovement);
		bool HitCheck(Walkable* playerLocation, Walkable* location) const;
		
		std::weak_ptr<Walkable> m_CurrentLocation;
		Side m_Side;
	};
}
