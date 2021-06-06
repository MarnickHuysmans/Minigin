#pragma once
#include "StateMachine.h"
#include "CoilyStates.h"
#include "Component.h"

namespace qbert
{
	class EnemySpawner;
	class MoveDown;

	class Coily : public dae::Component
	{
	public:
		Coily(const std::weak_ptr<EnemySpawner>& enemySpawner, bool player, float moveTime = 0.5f);
		virtual ~Coily() = default;

		void Start() override;
		void Update() override;

		std::shared_ptr<CoilyAIState> GetAiState() const;
		std::shared_ptr<CoilyPlayerState> GetPlayerState() const;

		std::weak_ptr<EnemySpawner> GetEnemySpawner() const;
		float GetMoveTime() const;
		bool IsPlayer() const;

	private:
		StateMachine m_StateMachine;
		std::shared_ptr<CoilyAIState> m_AiState;
		std::shared_ptr<CoilyPlayerState> m_PlayerState;

		std::weak_ptr<EnemySpawner> m_EnemySpawner;
		float m_MoveTime;
		bool m_IsPlayer;
	};
}
