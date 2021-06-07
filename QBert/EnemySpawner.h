#pragma once
#include <Component.h>
#include <vector>

#include "LevelObserver.h"
#include "MovementObserver.h"
#include "QbertObserver.h"

namespace qbert
{
	class Movement;
	class Level;

	class EnemySpawner : public dae::Component, public LevelObserver, public QbertObserver, public MovementObserver,
	                     public std::enable_shared_from_this<EnemySpawner>
	{
	public:
		EnemySpawner(std::weak_ptr<Level> level, std::vector<std::weak_ptr<Movement>> playerMovements,
		             bool coilyPlayer = false, float slickSamMinTime = 15.0f, float slickSamMaxTime = 30.0f,
		             float uggWrongWayMinTime = 7.0f, float uggWrongWayMaxTime = 15.0f, float coilyMinTime = 4.0f,
		             float coilyMaxTime = 8.0f);
		virtual ~EnemySpawner() = default;

		void Update() override;

		const std::vector<std::weak_ptr<Movement>>& GetPlayerMovements() const;
		void AddScore(unsigned int score);

		void LevelDisc() override;
		void NextLevel() override;
		void GameComplete() override;

		void QbertLives(int lives) override;
		void QbertRespawn() override;

		void Fall() override;
		void Moved(std::weak_ptr<Movement> movement) override;

		void CoilyFall();

		const std::weak_ptr<Level>& GetLevel() const { return m_Level; }

	private:
		void SpawnSlickSam(float elapsed);
		void SpawnUggWrongWay(float elapsed);
		void SpawnCoily(float elapsed);
		void EraseEnemies(float elapsed);

		void DestroyEnemies();
		void DestroyCoily();

	private:
		std::vector<std::weak_ptr<dae::GameObject>> m_Enemies;
		std::vector<std::weak_ptr<dae::GameObject>> m_Coily;
		std::vector<std::weak_ptr<Movement>> m_PlayerMovements;
		std::weak_ptr<Level> m_Level;
		float m_SlickSamTimer;
		float m_SlickSamTimeMin;
		float m_SlickSamTimeMax;
		float m_UggWrongWayTimer;
		float m_UggWrongWayTimeMin;
		float m_UggWrongWayTimeMax;
		float m_CoilyTimer;
		float m_CoilyTimeMin;
		float m_CoilyTimeMax;
		float m_QbertSpawnDelay;
		float m_EraseTimer;
		float m_EraseTime;
		bool m_CoilyPlayer;

		bool m_DestroyEnemies;
		bool m_DestroyCoily;
		bool m_Spawning;

		static unsigned int m_CoilyScore;
	};
}
