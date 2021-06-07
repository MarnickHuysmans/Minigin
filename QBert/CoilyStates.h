#pragma once
#include "IState.h"
#include "LevelObserver.h"
#include "MovementObserver.h"

#include <memory>

namespace dae
{
	class RenderComponent;
}

namespace qbert
{
	class Coily;
	class StateMachine;
	class Movement;
	class MoveDown;
	class PlayerInput;
	class Walkable;

	class CoilyMoveDownState : public IState, public MovementObserver,
	                           public std::enable_shared_from_this<CoilyMoveDownState>
	{
	public:
		CoilyMoveDownState(StateMachine& stateMachine, Coily* coily);
		virtual ~CoilyMoveDownState() = default;

		void Initialize() override;
		void Enter() override;
		void Update() override;
		void Exit() override;

		void Fall() override;
		void Moved(std::weak_ptr<Movement> movement) override;

	private:
		StateMachine& m_StateMachine;
		Coily* m_Coily;
		std::weak_ptr<MoveDown> m_MoveDown;
		std::weak_ptr<dae::RenderComponent> m_RenderComponent;
		std::weak_ptr<Movement> m_Movement;
	};

	class CoilyAIState : public IState, public LevelObserver, public std::enable_shared_from_this<CoilyAIState>
	{
	public:
		CoilyAIState(StateMachine& stateMachine, Coily* coily);
		virtual ~CoilyAIState() = default;

		void Initialize() override;
		void Enter() override;
		void Update() override;
		void Exit() override;

		void LevelDisc() override;
		void NextLevel() override;
		void GameComplete() override;

	private:
		void MoveTo(const std::weak_ptr<Movement>& playerMovement, const std::shared_ptr<Walkable>& enemyWalkable,
		            const std::shared_ptr<Movement>& enemyMovement);

		StateMachine& m_StateMachine;
		Coily* m_Coily;
		std::weak_ptr<Movement> m_Movement;
		float m_MoveTimer;
	};

	class CoilyPlayerState : public IState, public MovementObserver,
	                         public std::enable_shared_from_this<CoilyPlayerState>
	{
	public:
		CoilyPlayerState(StateMachine& stateMachine, Coily* coily);
		virtual ~CoilyPlayerState() = default;

		void Initialize() override;
		void Enter() override;
		void Update() override;
		void Exit() override;

		void Fall() override;
		void Moved(std::weak_ptr<Movement> movement) override;

	private:
		void RegisterMovement();

		StateMachine& m_StateMachine;
		Coily* m_Coily;
		std::weak_ptr<Movement> m_Movement;
		std::weak_ptr<PlayerInput> m_PlayerInput;
	};
}
