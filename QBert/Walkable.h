#pragma once
#include <Component.h>
#include <memory>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

namespace qbert
{
	enum class Direction;
	enum class Side;
	class Level;

	class Walkable : public dae::Component
	{
	public:
		enum class WalkableType
		{
			LevelCube,
			Disc
		};
		
		Walkable(std::weak_ptr<Level> level, int row, int col, const glm::vec2& topMiddle = glm::vec2(0, 0), const glm::vec2& rightMiddle = glm::vec2(0, 0), const glm::vec2& leftMiddle = glm::vec2(0, 0));
		virtual ~Walkable() = 0;

		const glm::vec2& GetTopMiddle() const { return m_TopMiddle; }
		const glm::vec2& GetRightMiddle() const { return m_RightMiddle; }
		const glm::vec2& GetLeftMiddle() const { return m_LeftMiddle; }

		std::weak_ptr<Walkable> GetWalkable(Side side, Direction direction);

		int GetRow() const { return m_Row; }
		int GetColumn() const { return m_Column; }
		int GetLevelSize() const;

		virtual WalkableType GetWalkableType() = 0;

	protected:
		glm::vec2 m_TopMiddle;
		glm::vec2 m_RightMiddle;
		glm::vec2 m_LeftMiddle;

		std::weak_ptr<Level> m_Level;

		int m_Row;
		int m_Column;
	};
}
