#pragma once
#include <Component.h>

#include "QbertObserver.h"

namespace dae
{
	class UIComponent;
}

namespace qbert
{
	class GameOver : public dae::Component, public QbertObserver
	{
	public:
		GameOver() = default;
		virtual ~GameOver() = default;

		void Start() override;
		
		void QbertLives(int lives) override;
		void QbertRespawn() override;
		
	private:
		std::weak_ptr<dae::UIComponent> m_UiComponent;
	};
}
