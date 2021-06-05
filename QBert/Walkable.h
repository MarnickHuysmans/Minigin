#pragma once
#include <Component.h>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

namespace qbert
{
	class Movement;
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

		Walkable(std::weak_ptr<Level> level, int row, int col, const glm::vec3& topMiddle = glm::vec3(0, 0, 0), const glm::vec3& rightMiddle = glm::vec3(0, 0, 0), const glm::vec3& leftMiddle = glm::vec3(0, 0, 0));
		virtual ~Walkable() = 0;

		const glm::vec3& GetMiddleOffset(Side side) const;

		std::weak_ptr<Walkable> GetWalkable(Side side, Direction direction);

		int GetRow() const { return m_Row; }
		int GetColumn() const { return m_Column; }
		int GetLevelSize() const;

		virtual WalkableType GetWalkableType() = 0;

		virtual void StepOn(Movement*) {}

		virtual void NextLevel() {}

	private:
		glm::vec3 m_TopMiddle;
		glm::vec3 m_RightMiddle;
		glm::vec3 m_LeftMiddle;

	protected:
		std::weak_ptr<Level> m_Level;

	private:
		int m_Row;
		int m_Column;
	};
}
