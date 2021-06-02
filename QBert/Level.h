#pragma once
#include <Component.h>
#include <vector>

namespace qbert
{
	enum class Side;
	enum class Direction;
	class Walkable;

	//The actual level is 2 sizes bigger but the LevelIndex(int row, int col) will return the index for the cubes when using row: 0-6 and col: 0-(0,1,2,3,4,5,6) depending on the row.
	class Level final : public dae::Component
	{
	public:
		Level(int levelSize);
		~Level() = default;

		void SetLevel(const std::weak_ptr<Walkable>& walkable, int row, int col);

		std::weak_ptr<Walkable> GetTopCube() const;
		std::weak_ptr<Walkable> GetTopRightCube() const;
		std::weak_ptr<Walkable> GetTopLeftCube() const;
		std::weak_ptr<Walkable> GetBottomRightCube() const;
		std::weak_ptr<Walkable> GetBottomLeftCube() const;

		std::weak_ptr<Walkable> GetWalkable(Side side, Direction direction, int row, int col) const;

		static size_t LevelIndex(int row, int col);

		int GetLevelSize() const { return m_LevelSize; }

	private:
		std::weak_ptr<Walkable> GetWalkableTop(Direction direction, int row, int col) const;
		std::weak_ptr<Walkable> GetWalkableRight(Direction direction, int row, int col) const;
		std::weak_ptr<Walkable> GetWalkableLeft(Direction direction, int row, int col) const;

		const int m_LevelSize;
		std::vector<std::weak_ptr<Walkable>> m_Level;
	};
}
