#include "Movement.h"
#include "Walkable.h"
#include "GameObject.h"
#include "GameTime.h"
#include "MovementObserver.h"

qbert::Movement::Movement(const std::weak_ptr<Walkable>& currentWalkable, const glm::vec3& positionOffset, Side side, float moveTime, bool enemy) :
	m_CurrentWalkable(currentWalkable),
	m_StartWalkable(currentWalkable),
	m_Transform(nullptr),
	m_PositionOffset(positionOffset),
	m_Side(side),
	m_MoveTimer(0),
	m_MoveTime(moveTime),
	m_CanMove(true),
	m_Enemy(enemy)
{
}

void qbert::Movement::Start()
{
	m_Transform = &GetGameObject()->GetTransform();
	if (m_CurrentWalkable.expired())
	{
		Fall();
		return;
	}
	MoveToCurrent();
}

void qbert::Movement::Update()
{
	if (m_MoveTimer <= 0)
	{
		return;
	}
	m_MoveTimer -= dae::GameTime::GetInstance().GetDeltaTime();
	if (m_MoveTimer <= 0)
	{
		SetCurrentWalkable(m_NextWalkable);
	}
}

void qbert::Movement::Move(Direction direction)
{
	if (!m_CanMove || m_MoveTimer > 0)
	{
		return;
	}

	m_NextWalkable = m_CurrentWalkable.lock()->GetWalkable(m_Side, direction);
	if (m_Enemy && !m_NextWalkable.expired() && m_NextWalkable.lock()->GetWalkableType() != Walkable::WalkableType::LevelCube)
	{
		m_NextWalkable = std::weak_ptr<Walkable>();
	}
	m_MoveTimer = m_MoveTime;
}

void qbert::Movement::CanMove(bool canMove)
{
	m_CanMove = canMove;
	if (!m_CanMove)
	{
		m_MoveTimer = 0;
	}
}

void qbert::Movement::SetCurrentWalkable(const std::weak_ptr<Walkable>& walkable)
{
	m_CurrentWalkable = walkable;
	if (m_CurrentWalkable.expired() || !m_CurrentWalkable.lock()->ActiveInScene())
	{
		Fall();
		return;
	}
	MoveToCurrent();
	m_CurrentWalkable.lock()->StepOn(this);
}

void qbert::Movement::Respawn()
{
	if (m_CurrentWalkable.expired())
	{
		if (m_StartWalkable.expired())
		{
			return;
		}
		m_CurrentWalkable = m_StartWalkable;
	}
	MoveToCurrent();
	m_CanMove = true;
	m_MoveTimer = 0;
}

void qbert::Movement::AddObserver(const std::weak_ptr<MovementObserver>& observer)
{
	m_MovementObservers.push_back(observer);
}

void qbert::Movement::MoveToCurrent()
{
	auto current = m_CurrentWalkable.lock();

	current->GetGameObject()->AddChild(GetGameObject());

	auto& middle = current->GetMiddleOffset(m_Side);
	m_Transform->SetLocalPosition(middle + m_PositionOffset);
}

void qbert::Movement::Fall()
{
	m_CurrentWalkable = std::weak_ptr<Walkable>();
	if (m_MovementObservers.empty())
	{
		GetGameObject()->Destroy();
		return;
	}
	for (auto& movementObserver : m_MovementObservers)
	{
		if (movementObserver.expired())
		{
			continue;
		}
		movementObserver.lock()->Fall();
	}
}
