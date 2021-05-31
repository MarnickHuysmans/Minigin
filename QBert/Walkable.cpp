#include "Walkable.h"

#include <utility>
#include "Level.h"

qbert::Walkable::Walkable(std::weak_ptr<Level> level, int row, int col, const glm::vec2& topMiddle, const glm::vec2& rightMiddle, const glm::vec2& leftMiddle) :
	m_TopMiddle(topMiddle),
	m_RightMiddle(rightMiddle),
	m_LeftMiddle(leftMiddle),
	m_Level(std::move(level)),
	m_Row(row),
	m_Column(col)

{
}

qbert::Walkable::~Walkable()
{
}

std::weak_ptr<qbert::Walkable> qbert::Walkable::GetWalkable(Side side, Direction direction)
{
	if (m_Level.expired())
	{
		return std::weak_ptr<Walkable>();
	}
	return m_Level.lock()->GetWalkable(side, direction, m_Row, m_Column);
}

int qbert::Walkable::GetLevelSize() const
{
	if (m_Level.expired())
	{
		return 0;
	}
	return m_Level.lock()->GetLevelSize();
}
