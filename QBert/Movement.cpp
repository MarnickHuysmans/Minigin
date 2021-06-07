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
	m_StartMoveTime(moveTime),
	m_Moving(false),
	m_CanMove(true),
	m_Enemy(enemy)
{
}

void qbert::Movement::Start()
{
	if (m_GameObject.expired())
	{
		return;
	}
	m_Transform = &m_GameObject.lock()->GetTransform();
	if (m_CurrentWalkable.expired())
	{
		Fall();
		return;
	}
	MoveToCurrent();
}

void qbert::Movement::Update()
{
	if (!m_Moving)
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
	m_Moving = true;
}

void qbert::Movement::CanMove(bool canMove)
{
	m_CanMove = canMove;
	if (!m_CanMove)
	{
		m_Moving = false;
	}
}

void qbert::Movement::SetCurrentWalkable(const std::weak_ptr<Walkable>& walkable)
{
	m_Moving = false;
	m_CurrentWalkable = walkable;
	if (m_CurrentWalkable.expired() || !m_CurrentWalkable.lock()->ActiveInScene())
	{
		Fall();
		return;
	}
	MoveToCurrent();
	m_CurrentWalkable.lock()->StepOn(this);
}

void qbert::Movement::Respawn(bool toStart)
{
	if (m_CurrentWalkable.expired() || toStart)
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
	m_Moving = false;
}

void qbert::Movement::AddObserver(const std::weak_ptr<MovementObserver>& observer)
{
	m_MovementObservers.push_back(observer);
}

void qbert::Movement::MoveToCurrent()
{
	auto current = m_CurrentWalkable.lock();

	auto weakGameObject = current->GetGameObject();
	if (weakGameObject.expired() || m_GameObject.expired())
	{
		return;
	}

	weakGameObject.lock()->AddChild(m_GameObject.lock());

	auto& middle = current->GetMiddleOffset(m_Side);
	m_Transform->SetLocalPosition(middle + m_PositionOffset);

	m_MovementObservers.erase(std::remove_if(std::begin(m_MovementObservers), std::end(m_MovementObservers), [this](const std::weak_ptr<MovementObserver>& observer)
		{
			if (observer.expired())
			{
				return true;
			}
			observer.lock()->Moved(weak_from_this());
			return false;
		}), std::end(m_MovementObservers));
}

void qbert::Movement::Fall()
{
	m_CurrentWalkable = std::weak_ptr<Walkable>();
	for (auto& movementObserver : m_MovementObservers)
	{
		if (movementObserver.expired())
		{
			continue;
		}
		movementObserver.lock()->Fall();
	}
}
