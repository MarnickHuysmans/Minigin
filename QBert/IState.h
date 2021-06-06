#pragma once

namespace qbert
{
	class IState
	{
	public:
		virtual ~IState() = 0;

		virtual void Initialize() = 0;
		virtual void Enter() = 0;
		virtual void Update() = 0;
		virtual void Exit() = 0;
	};
}
