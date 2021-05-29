#include "Level.h"

const std::weak_ptr<qbert::Walkable>& qbert::Level::GetTopRightCube()
{
	if (m_TopCube.expired())
	{
		return std::weak_ptr<qbert::Walkable>();
	}
	return m_TopCube.lock()->GetTopRight();
}

const std::weak_ptr<qbert::Walkable>& qbert::Level::GetTopLeftCube()
{
	if (m_TopCube.expired())
	{
		return std::weak_ptr<qbert::Walkable>();
	}
	return m_TopCube.lock()->GetTopDown();
}
