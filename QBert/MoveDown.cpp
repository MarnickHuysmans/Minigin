#include "MoveDown.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Movement.h"

qbert::MoveDown::MoveDown(Direction direction1, Direction direction2, float moveTime) :
	m_MoveTimer(moveTime),
	m_MoveTime(moveTime),
	m_Direction1(direction1),
	m_Direction2(direction2)
{
}

void qbert::MoveDown::Start()
{
	if (m_GameObject.expired())
	{
		return;
	}
	m_Movement = m_GameObject.lock()->GetComponent<Movement>();
}

void qbert::MoveDown::Update()
{
	m_MoveTimer -= dae::GameTime::GetInstance().GetDeltaTime();
	if (m_MoveTimer > 0)
	{
		return;
	}
	m_MoveTimer += m_MoveTime;
	if (m_Movement.expired())
	{
		return;
	}
	bool direction = rand() % 2;
	m_Movement.lock()->Move(direction ? m_Direction1 : m_Direction2);
}
