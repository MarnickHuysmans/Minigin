#pragma once
#include <memory>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#include "Qbert.h"

namespace dae {
	enum class Player;
	class Scene;
}

namespace qbert
{
	class Walkable;
	class QbertFactory
	{
	public:
		static const std::weak_ptr<Qbert> CreateQbert(const std::weak_ptr<Walkable>& startCube, dae::Player player);

	private:
		static glm::vec3 m_PositionOffset;
	};
}
