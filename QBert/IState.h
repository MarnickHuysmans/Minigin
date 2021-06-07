#pragma once

namespace qbert
{
	class IState
	{
	public:
		IState() = default;
		virtual ~IState() = default;

		virtual void Initialize() = 0;
		virtual void Enter() = 0;
		virtual void Update() = 0;
		virtual void Exit() = 0;
	};
}
