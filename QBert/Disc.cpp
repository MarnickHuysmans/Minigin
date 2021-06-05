#include "Disc.h"
#include "Movement.h"
#include "GameTime.h"
#include "GameObject.h"
#include "Level.h"

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

void qbert::Disc::StepOn(qbert::Movement* movement)
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
}

void qbert::Disc::DoneMoving() const
{
	m_GameObject->SetActive(false);
	if (m_Movement == nullptr || m_Level.expired())
	{
		return;
	}

	m_Movement->CanMove(true);
	m_Movement->SetCurrentWalkable(m_Level.lock()->GetTopCube());
}
