#pragma once
#include "Walkable.h"

namespace qbert
{
	class Disc : public Walkable
	{
	public:
		Disc(const std::weak_ptr<Level>& level, int row, int col, const glm::vec3& topMiddle, float moveTime = 1);
		virtual ~Disc() = default;

		void Update() override;

		WalkableType GetWalkableType() override;

		void StepOn(Movement* movement) override;

	private:
		void DoneMoving() const;
		
		Movement* m_Movement;
		
		float m_MoveTimer;
		float m_MoveTime;
	};
}
