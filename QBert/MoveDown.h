#pragma once
#include <Component.h>

#include "LevelEnums.h"

namespace qbert
{
	class Movement;

	class MoveDown : public dae::Component
	{
	public:
		MoveDown(Direction direction1, Direction direction2, float moveTime = 2.0f);
		virtual ~MoveDown() = default;

		void Start() override;
		void Update() override;

	protected:
		std::weak_ptr<Movement> m_Movement;

		float m_MoveTimer;
		float m_MoveTime;

	private:
		Direction m_Direction1;
		Direction m_Direction2;
	};
}
