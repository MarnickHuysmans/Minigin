#include "Level.h"
#include "Walkable.h"
#include "LevelEnums.h"

qbert::Level::Level(int levelSize) :
	m_Level(LevelIndex(levelSize, -1)),
	m_LevelSize(levelSize),
	m_Win(LevelIndex(levelSize - 2, -1)),
	m_WinCounter(0)
{
}

void qbert::Level::SetLevel(const std::weak_ptr<Walkable>& walkable, int row, int col)
{
	auto index = LevelIndex(row, col);
	if (index > m_Level.size())
	{
		return;
	}
	m_Level[index] = walkable;
}

std::weak_ptr<qbert::Walkable> qbert::Level::GetTopCube() const
{
	return m_Level[LevelIndex(0, 0)];
}

std::weak_ptr<qbert::Walkable> qbert::Level::GetTopRightCube() const
{
	return m_Level[LevelIndex(1, 1)];
}

std::weak_ptr<qbert::Walkable> qbert::Level::GetTopLeftCube() const
{
	return m_Level[LevelIndex(1, 0)];
}

std::weak_ptr<qbert::Walkable> qbert::Level::GetBottomRightCube() const
{
	return m_Level[LevelIndex(m_LevelSize - 1, m_LevelSize - 1)];
}

std::weak_ptr<qbert::Walkable> qbert::Level::GetBottomLeftCube() const
{
	return m_Level[LevelIndex(m_LevelSize - 1, 0)];
}

std::weak_ptr<qbert::Walkable> qbert::Level::GetWalkable(Side side, Direction direction, int row, int col) const
{
	switch (side)
	{
	case Side::Top:
		return GetWalkableTop(direction, row, col);
	case Side::Right:
		return GetWalkableRight(direction, row, col);
	case Side::Left:
		return GetWalkableLeft(direction, row, col);
	default:
		return std::weak_ptr<Walkable>();
	}
}

size_t qbert::Level::LevelIndex(int row, int col)
{
	row += 2;
	col += 1;
	return (row * row - row) / 2 + row + col;
}

void qbert::Level::Done()
{
	++m_WinCounter;
}

void qbert::Level::Undone()
{
	--m_WinCounter;
}

std::weak_ptr<qbert::Walkable> qbert::Level::GetWalkableTop(Direction direction, int row, int col) const
{
	switch (direction)
	{
	case Direction::Up:
		return m_Level[LevelIndex(row - 1, col)];
	case Direction::Right:
		if (row >= m_LevelSize - 1)
		{
			return std::weak_ptr<Walkable>();
		}
		return m_Level[LevelIndex(row + 1, col + 1)];
	case Direction::Down:
		if (row >= m_LevelSize - 1)
		{
			return std::weak_ptr<Walkable>();
		}
		return m_Level[LevelIndex(row + 1, col)];
	case Direction::Left:
		return m_Level[LevelIndex(row - 1, col - 1)];
	default:
		return std::weak_ptr<Walkable>();
	}
}

std::weak_ptr<qbert::Walkable> qbert::Level::GetWalkableRight(Direction direction, int row, int col) const
{
	switch (direction)
	{
	case Direction::Up:
		return m_Level[LevelIndex(row - 1, col - 1)];
	case Direction::Right:
		return m_Level[LevelIndex(row, col + 1)];
	case Direction::Down:
		if (row >= m_LevelSize - 1)
		{
			return std::weak_ptr<Walkable>();
		}
		return m_Level[LevelIndex(row + 1, col + 1)];
	case Direction::Left:
		return m_Level[LevelIndex(row, col - 1)];
	default:
		return std::weak_ptr<Walkable>();
	}
}

std::weak_ptr<qbert::Walkable> qbert::Level::GetWalkableLeft(Direction direction, int row, int col) const
{
	switch (direction)
	{
	case Direction::Up:
		return m_Level[LevelIndex(row - 1, col)];
	case Direction::Right:
		return m_Level[LevelIndex(row, col + 1)];
	case Direction::Down:
		if (row >= m_LevelSize - 1)
		{
			return std::weak_ptr<Walkable>();
		}
		return m_Level[LevelIndex(row + 1, col)];
	case Direction::Left:
		return m_Level[LevelIndex(row, col - 1)];
	default:
		return std::weak_ptr<Walkable>();
	}
}
