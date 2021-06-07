#pragma once
#include <vector>

#include "Walkable.h"

namespace qbert
{
	class DiscObserver;

	class Disc : public Walkable
	{
	public:
		Disc(const std::weak_ptr<Level>& level, int row, int col, const glm::vec3& topMiddle, float moveTime = 6.0f);
		virtual ~Disc() = default;

		void Update() override;

		WalkableType GetWalkableType() override;

		void StepOn(Movement* movement) override;

		void NextLevel() override;

		void AddObserver(const std::weak_ptr<DiscObserver>& observer);

	private:
		void DoneMoving();

		std::vector<std::weak_ptr<DiscObserver>> m_DiscObservers;

		Movement* m_Movement;

		float m_MoveTimer;
		float m_MoveTime;

		static unsigned int m_DiscScore;
	};
}
