#include "Qbert.h"
#include "QbertObserver.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Movement.h"
#include "ServiceLocator.h"

qbert::Qbert::Qbert(int lives, float respawnTime) :
	m_Lives(lives),
	m_RespawnTimer(0),
	m_RespawnTime(respawnTime),
	m_NotifyLife(false)
{
}

void qbert::Qbert::Start()
{
	if (m_GameObject.expired())
	{
		return;
	}
	auto gameObject = m_GameObject.lock();
	m_Movement = gameObject->GetComponent<Movement>();
	m_RenderComponent = gameObject->GetComponent<dae::RenderComponent>();
}

void qbert::Qbert::Update()
{
	if (m_NotifyLife)
	{
		NotifyObservers([this](QbertObserver* observer) {observer->QbertLives(m_Lives); });
		m_NotifyLife = false;
	}
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
		Damage();
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
	m_NotifyLife = true;
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
	dae::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sound/QbertOverTheEdge.wav");
	Damage();
}

void qbert::Qbert::Moved(std::weak_ptr<Movement>)
{
	dae::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sound/Hop.wav");
}

void qbert::Qbert::AddObserver(const std::weak_ptr<QbertObserver>& observer)
{
	m_QbertObservers.push_back(observer);
}

void qbert::Qbert::LevelDisc()
{
}

void qbert::Qbert::NextLevel()
{
	if (m_Movement.expired())
	{
		return;
	}
	m_Movement.lock()->Respawn(true);
}

void qbert::Qbert::GameComplete()
{
	if (m_Movement.expired())
	{
		return;
	}
	m_Movement.lock()->CanMove(false);
}

void qbert::Qbert::QbertLives(int lives)
{
	if (lives != 0 || m_Movement.expired())
	{
		return;
	}
	m_Movement.lock()->CanMove(false);
}

void qbert::Qbert::QbertRespawn()
{
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
