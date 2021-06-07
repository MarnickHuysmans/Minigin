#pragma once
#include <Component.h>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#include <vector>
#include "LevelEnums.h"

namespace dae
{
	class Transform;
}

namespace qbert
{
	class Walkable;
	class MovementObserver;

	class Movement : public dae::Component, public std::enable_shared_from_this<Movement>
	{
	public:
		Movement(const std::weak_ptr<Walkable>& currentWalkable, const glm::vec3& positionOffset, Side side,
		         float moveTime = 0.75f, bool enemy = true);
		virtual ~Movement() = default;

		void Start() override;
		void Update() override;

		void Move(Direction direction);
		bool CanMove() const { return m_CanMove; }
		void CanMove(bool canMove);
		std::weak_ptr<Walkable> GetCurrentWalkable() const { return m_CurrentWalkable; }
		void SetCurrentWalkable(const std::weak_ptr<Walkable>& walkable);
		void Respawn(bool toStart = false);

		void AddObserver(const std::weak_ptr<MovementObserver>& observer);

		void SetPositionOffset(const glm::vec3& offset) { m_PositionOffset = offset; }
		void SetMoveTime(float moveTime) { m_MoveTime = moveTime; }
		void ResetMoveTime() { m_MoveTime = m_StartMoveTime; }

	private:
		void MoveToCurrent();
		void Fall();

		std::vector<std::weak_ptr<MovementObserver>> m_MovementObservers;

		std::weak_ptr<Walkable> m_CurrentWalkable;
		std::weak_ptr<Walkable> m_NextWalkable;
		std::weak_ptr<Walkable> m_StartWalkable;

		dae::Transform* m_Transform;

		glm::vec3 m_PositionOffset;

		Side m_Side;

		float m_MoveTimer;
		float m_MoveTime;
		float m_StartMoveTime;
		bool m_Moving;

		bool m_CanMove;
		bool m_Enemy;
	};
}
