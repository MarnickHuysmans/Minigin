#pragma once
#include <memory>

#include "IState.h"
#include "LevelObserver.h"
#include "MovementObserver.h"

namespace qbert
{
	class Coily;
	class StateMachine;
	class Movement;
	class MoveDown;
	class PlayerInput;

	class CoilyMoveDownState : public IState, public MovementObserver, public std::enable_shared_from_this<CoilyMoveDownState>
	{
	public:
		CoilyMoveDownState(StateMachine& stateMachine, Coily* coily);

		void Initialize() override;
		void Enter() override;
		void Update() override;
		void Exit() override;

		void Fall() override;
		void Moved(Movement* movement) override;

	private:
		StateMachine& m_StateMachine;
		Coily* m_Coily;
		std::weak_ptr<MoveDown> m_MoveDown;
	};

	class CoilyAIState : public IState, public LevelObserver, public std::enable_shared_from_this<CoilyAIState>
	{
	public:
		CoilyAIState(StateMachine& stateMachine, Coily* coily);

		void Initialize() override;
		void Enter() override;
		void Update() override;
		void Exit() override;

		void LevelDisc() override;
		void NextLevel() override;
		void GameComplete() override;
		
	private:
		StateMachine& m_StateMachine;
		Coily* m_Coily;
		std::weak_ptr<Movement> m_Movement;
		float m_MoveTimer;
	};

	class CoilyPlayerState : public IState, public MovementObserver, public std::enable_shared_from_this<CoilyPlayerState>
	{
	public:
		CoilyPlayerState(StateMachine& stateMachine, Coily* coily);

		void Initialize() override;
		void Enter() override;
		void Update() override;
		void Exit() override;

		void Fall() override;
		void Moved(Movement* movement) override;
		
	private:
		void RegisterMovement();
		
		StateMachine& m_StateMachine;
		Coily* m_Coily;
		std::weak_ptr<Movement> m_Movement;
		std::weak_ptr<PlayerInput> m_PlayerInput;
	};
}
