#include "Qbert.h"
#include "QbertObserver.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Movement.h"

qbert::Qbert::Qbert() :
	m_Lives(3),
	m_RespawnTimer(0),
	m_RespawnTime(1)
{
}

void qbert::Qbert::Start()
{
	m_Movement = m_GameObject->GetComponent<Movement>();
	m_RenderComponent = m_GameObject->GetComponent<dae::RenderComponent>();
}

void qbert::Qbert::Update()
{
	if (m_Lives <= 0 || m_RespawnTimer <= 0)
	{
		return;
	}
	m_RespawnTimer -= dae::GameTime::GetInstance().GetDeltaTime();
	if (m_RespawnTimer > 0)
	{
		return;
	}
	NotifyObservers(&QbertObserver::QbertRespawn);
	if (!m_RenderComponent.expired())
	{
		m_RenderComponent.lock()->SetActive(true);
	}
	if (m_Movement.expired())
	{
		return;
	}
	auto movement = m_Movement.lock();
	movement->CanMove(true);
	movement->Respawn();
}

void qbert::Qbert::Damage()
{
	if (m_RespawnTimer > 0)
	{
		return;
	}
	--m_Lives;
	NotifyObservers([this](QbertObserver* observer) {observer->QbertLives(m_Lives); });
	m_RespawnTimer = m_RespawnTime;
	if (!m_RenderComponent.expired())
	{
		m_RenderComponent.lock()->SetActive(false);
	}
	if (m_Movement.expired())
	{
		return;
	}
	m_Movement.lock()->CanMove(false);
}

void qbert::Qbert::Fall()
{
	Damage();
}

void qbert::Qbert::AddObserver(const std::weak_ptr<QbertObserver>& observer)
{
	m_QbertObservers.push_back(observer);
}

void qbert::Qbert::NotifyObservers(std::function<void(QbertObserver*)> observerFunction)
{
	m_QbertObservers.erase(std::remove_if(std::begin(m_QbertObservers), std::end(m_QbertObservers),
		[&observerFunction](const std::weak_ptr<QbertObserver>& observer)
		{
			if (observer.expired())
			{
				return true;
			}
			observerFunction(observer.lock().get());
			return false;
		}), std::end(m_QbertObservers));
}
