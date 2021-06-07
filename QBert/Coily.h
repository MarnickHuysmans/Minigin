#pragma once
#include "StateMachine.h"
#include "CoilyStates.h"
#include "Component.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

namespace dae
{
	class Texture2D;
}

namespace qbert
{
	class EnemySpawner;
	class MoveDown;

	class Coily : public dae::Component, public MovementObserver
	{
	public:
		Coily(const std::weak_ptr<EnemySpawner>& enemySpawner, bool player, const glm::vec3& offset, const std::shared_ptr<dae::Texture2D>& texture, float moveTime = 2.0f);
		virtual ~Coily() = default;

		void Start() override;
		void Update() override;

		std::shared_ptr<CoilyAIState> GetAiState() const;
		std::shared_ptr<CoilyPlayerState> GetPlayerState() const;

		std::weak_ptr<EnemySpawner> GetEnemySpawner() const;
		float GetMoveTime() const;
		bool IsPlayer() const;
		const glm::vec3& GetOffset() const;
		const std::shared_ptr<dae::Texture2D>& GetTexture() const;

		void Fall() override;
		void Moved(std::weak_ptr<Movement> movement) override;
		
	private:
		StateMachine m_StateMachine;
		std::shared_ptr<CoilyAIState> m_AiState;
		std::shared_ptr<CoilyPlayerState> m_PlayerState;

		glm::vec3 m_Offset;
		std::shared_ptr<dae::Texture2D> m_Texture;
		std::weak_ptr<EnemySpawner> m_EnemySpawner;
		float m_MoveTime;
		bool m_IsPlayer;
	};
}
