#pragma once
#include <memory>

namespace qbert
{
	class Movement;

	class MovementObserver
	{
	public:
		MovementObserver() = default;
		virtual ~MovementObserver() = default;
		virtual void Fall() = 0;
		virtual void Moved(std::weak_ptr<Movement> movement) = 0;
	};
}
