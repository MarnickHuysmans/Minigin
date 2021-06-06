#pragma once

namespace qbert
{
	class DiscObserver
	{
	public:
		DiscObserver() = default;
		virtual ~DiscObserver() = default;
		virtual void DiscDoneMoving() = 0;
	};
}
