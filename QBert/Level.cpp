#include "Level.h"

#include <numeric>

#include "Walkable.h"
#include "LevelEnums.h"
#include "GameObject.h"
#include "LevelObserver.h"
#include "Score.h"

unsigned int qbert::Level::m_CubeScore = 25;

qbert::Level::Level(int levelSize, int discAmount, std::weak_ptr<Score> score) :
	m_Level(LevelIndex(levelSize, -1)),
	m_Score(score),
	m_LevelSize(levelSize),
	m_DiscAmount(discAmount),
	m_Win(LevelIndex(levelSize - 2, -1)),
	m_WinCounter(0),
	m_LevelCounter(0),
	m_MaxLevel(2)
{
}

void qbert::Level::Start()
{
	ActivateDiscs();
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

void qbert::Level::CubeDone()
{
	++m_WinCounter;
	AddScore(m_CubeScore);
	if (m_WinCounter >= m_Win)
	{
		if (m_LevelCounter >= m_MaxLevel)
		{
			NotifyObservers(&LevelObserver::GameComplete);
			return;
		}
		NextLevel();
	}
}

void qbert::Level::CubeActivated()
{
	AddScore(m_CubeScore);
}

void qbert::Level::CubeUndone()
{
	--m_WinCounter;
}

void qbert::Level::DiscDoneMoving()
{
	NotifyObservers(&LevelObserver::LevelDisc);
}

void qbert::Level::AddScore(unsigned score)
{
	if (m_Score.expired())
	{
		return;
	}
	m_Score.lock()->AddScore(score);
}

void qbert::Level::AddObserver(const std::weak_ptr<LevelObserver>& observer)
{
	m_LevelObservers.push_back(observer);
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

void qbert::Level::ActivateDiscs()
{
	std::vector<int> leftDiscs(m_LevelSize - 1);
	std::vector<int> rightDiscs(m_LevelSize - 1);
	std::iota(std::begin(leftDiscs), std::end(leftDiscs), 0);
	std::iota(std::begin(rightDiscs), std::end(rightDiscs), 0);
	bool left = true;
	for (int i = 0; i < m_DiscAmount; ++i)
	{
		unsigned int index = rand() % (left ? leftDiscs.size() : rightDiscs.size());
		int row = left ? leftDiscs[index] : rightDiscs[index];
		int col = left ? -1 : row + 1;

		auto disc = m_Level[LevelIndex(row, col)];
		if (disc.expired())
		{
			RemoveUsedDisc(left ? leftDiscs : rightDiscs, row);
			left = !left;
			continue;
		}
		disc.lock()->GetGameObject()->SetActive(true);

		left = !left;
	}
}

void qbert::Level::RemoveUsedDisc(std::vector<int>& discs, int row) const
{
	discs.erase(std::remove(std::begin(discs), std::end(discs), row), std::end(discs));
}

void qbert::Level::NextLevel()
{
	m_WinCounter = 0;
	++m_LevelCounter;
	for (auto& walkable : m_Level)
	{
		if (walkable.expired())
		{
			continue;
		}
		walkable.lock()->NextLevel();
	}
	ActivateDiscs();
	NotifyObservers(&LevelObserver::NextLevel);
}

void qbert::Level::NotifyObservers(std::function<void(LevelObserver*)> observerFunction)
{
	m_LevelObservers.erase(std::remove_if(std::begin(m_LevelObservers), std::end(m_LevelObservers),
		[&observerFunction](const std::weak_ptr<LevelObserver>& observer)
		{
			if (observer.expired())
			{
				return true;
			}
			observerFunction(observer.lock().get());
			return false;
		}), std::end(m_LevelObservers));
}
