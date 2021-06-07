#pragma once
#include "EnemyHit.h"

namespace qbert
{
	class Movement;

	class SlickSam : public EnemyHit
	{
	public:
		SlickSam(const std::weak_ptr<EnemySpawner>& enemySpawner);
		virtual ~SlickSam() = default;

	protected:
		void OnPlayerHit(Qbert* qbert) override;

	private:
		static unsigned int m_KillScore;
	};
}
