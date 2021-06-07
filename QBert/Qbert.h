#pragma once
#include <Component.h>
#include <functional>
#include <vector>

#include "LevelObserver.h"
#include "MovementObserver.h"
#include "QbertObserver.h"
#include "RenderComponent.h"

namespace qbert
{
	class Movement;

	class Qbert : public dae::Component, public MovementObserver, public LevelObserver, public QbertObserver
	{
	public:
		Qbert(int lives = 3, float respawnTime = 2.0f);
		virtual ~Qbert() = default;

		void Start() override;
		void Update() override;

		void Damage();

		void Fall() override;
		void Moved(std::weak_ptr<Movement> movement) override;

		void AddObserver(const std::weak_ptr<QbertObserver>& observer);

		int GetLives() const { return m_Lives; }

		void LevelDisc() override;
		void NextLevel() override;
		void GameComplete() override;

		void QbertLives(int lives) override;
		void QbertRespawn() override;

	private:
		void NotifyObservers(std::function<void(QbertObserver*)> observerFunction);

		std::vector<std::weak_ptr<QbertObserver>> m_QbertObservers;
		std::weak_ptr<Movement> m_Movement;
		std::weak_ptr<dae::RenderComponent> m_RenderComponent;
		int m_Lives;
		float m_RespawnTimer;
		float m_RespawnTime;
		bool m_NotifyLife;
	};
}
