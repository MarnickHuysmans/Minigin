#pragma once
#include "Component.h"

namespace qbert
{
	class LevelCubeActivator : dae::Component
	{
	public:

		LevelCubeActivator(bool forward);
		virtual ~LevelCubeActivator() = default;

		bool GetForward() const { return m_Forward; }

	private:
		bool m_Forward;
	};
}
