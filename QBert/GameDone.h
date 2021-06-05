#pragma once
#include <Component.h>

#include "LevelObserver.h"

namespace dae
{
	class UIComponent;
}

namespace qbert
{
	class GameDone : public dae::Component, public LevelObserver
	{
		public:
		GameDone() = default;
		virtual ~GameDone() = default;

		void Start() override;
		
		void NextLevel() override;
		void GameComplete() override;
		
	private:
		std::weak_ptr<dae::UIComponent> m_UiComponent;
	};
}
