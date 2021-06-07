#include "Walkable.h"

#include <utility>
#include "Level.h"
#include "LevelEnums.h"

qbert::Walkable::Walkable(std::weak_ptr<Level> level, int row, int col, const glm::vec3& topMiddle,
                          const glm::vec3& rightMiddle, const glm::vec3& leftMiddle) :
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

const glm::vec3& qbert::Walkable::GetMiddleOffset(Side side) const
{
	switch (side)
	{
	case Side::Top:
		return m_TopMiddle;
	case Side::Right:
		return m_RightMiddle;
	case Side::Left:
		return m_LeftMiddle;
	default:
		return m_TopMiddle;
	}
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

int qbert::Walkable::GetDistanceTo(const std::shared_ptr<Walkable>& other) const
{
	int otherRow = other->GetRow();
	int otherCol = other->GetColumn();

	return abs(m_Row - m_Column - (otherRow - otherCol)) + abs(m_Column - otherCol);
}
