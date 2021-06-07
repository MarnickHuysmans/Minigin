#pragma once
#include <Component.h>

#include "QbertObserver.h"

namespace dae
{
	class TextComponent;
}

namespace qbert
{
	class Lives : public dae::Component, public QbertObserver
	{
	public:
		Lives(int lives = 3);
		virtual ~Lives() = default;

		void Start() override;

		void QbertLives(int lives) override;
		void QbertRespawn() override;

	private:
		void SetText();
		
		std::weak_ptr<dae::TextComponent> m_TextComponent;
		int m_Lives;
	};
}
