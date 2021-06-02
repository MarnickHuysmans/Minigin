#pragma once
namespace qbert
{
	class MovementObserver
	{
	public:
		MovementObserver() = default;
		virtual ~MovementObserver() = default;
		virtual void Fall() = 0;
	};
}
