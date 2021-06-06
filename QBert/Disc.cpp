#include "Disc.h"
#include "Movement.h"
#include "GameTime.h"
#include "GameObject.h"
#include "Level.h"
#include "DiscObserver.h"

unsigned int qbert::Disc::m_DiscScore = 50;

qbert::Disc::Disc(const std::weak_ptr<Level>& level, int row, int col, const glm::vec3& topMiddle, float moveTime) :
	Walkable(level, row, col, topMiddle),
	m_Movement(nullptr),
	m_MoveTimer(0),
	m_MoveTime(moveTime)
{
}

void qbert::Disc::Update()
{
	if (m_MoveTimer <= 0)
	{
		return;
	}
	m_MoveTimer -= dae::GameTime::GetInstance().GetDeltaTime();
	if (m_MoveTimer <= 0)
	{
		DoneMoving();
	}
}

qbert::Walkable::WalkableType qbert::Disc::GetWalkableType()
{
	return WalkableType::Disc;
}

void qbert::Disc::StepOn(Movement* movement)
{
	if (movement == nullptr)
	{
		return;
	}
	m_Movement = movement;
	m_Movement->CanMove(false);
	m_MoveTimer = m_MoveTime;
}

void qbert::Disc::NextLevel()
{
	m_MoveTimer = 0;
	m_GameObject->SetActive(false);
	if (m_Level.expired())
	{
		return;
	}
	m_Level.lock()->AddScore(m_DiscScore);
}

void qbert::Disc::AddObserver(const std::weak_ptr<DiscObserver>& observer)
{
	m_DiscObservers.push_back(observer);
}

void qbert::Disc::DoneMoving()
{
	m_DiscObservers.erase(std::remove_if(std::begin(m_DiscObservers), std::end(m_DiscObservers), [](const std::weak_ptr<DiscObserver>& observer)
		{
			if (observer.expired())
			{
				return true;
			}
			observer.lock()->DiscDoneMoving();
			return false;
		}), std::end(m_DiscObservers));

	m_GameObject->SetActive(false);
	if (m_Movement == nullptr || m_Level.expired())
	{
		return;
	}

	m_Movement->CanMove(true);
	m_Movement->SetCurrentWalkable(m_Level.lock()->GetTopCube());
}
