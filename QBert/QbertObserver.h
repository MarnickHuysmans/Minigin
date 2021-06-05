#pragma once

namespace qbert
{
	class QbertObserver
	{
	public:
		QbertObserver() = default;
		virtual ~QbertObserver() = default;
		virtual void QbertLives(int lives) = 0;
		virtual void QbertRespawn() = 0;
	};
}
