#pragma once
#include <Component.h>
#include <functional>
#include <vector>
#include "MovementObserver.h"
#include "RenderComponent.h"

namespace qbert
{
	class QbertObserver;
	class Movement;

	class Qbert : public dae::Component, public MovementObserver
	{
	public:
		Qbert();
		virtual ~Qbert() = default;

		void Start() override;
		void Update() override;

		void Damage();

		void Fall() override;
		void Moved(Movement* movement) override;

		void AddObserver(const std::weak_ptr<QbertObserver>& observer);

		int GetLives() const { return m_Lives; }

	private:
		void NotifyObservers(std::function<void(QbertObserver*)> observerFunction);

		std::vector<std::weak_ptr<QbertObserver>> m_QbertObservers;
		std::weak_ptr<Movement> m_Movement;
		std::weak_ptr<dae::RenderComponent> m_RenderComponent;
		int m_Lives;
		float m_RespawnTimer;
		float m_RespawnTime;
	};
}
