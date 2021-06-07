#pragma once
#include <memory>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#include "Qbert.h"

namespace dae
{
	enum class Player;
	class Scene;
	class Font;
}

namespace qbert
{
	class Walkable;

	class QbertFactory
	{
	public:
		static const std::weak_ptr<Qbert> CreateQbert(dae::Scene& scene, const std::weak_ptr<Walkable>& startCube,
		                                              dae::Player player, const std::shared_ptr<dae::Font>& font);

	private:
		static glm::vec3 m_PositionOffset;
	};
}
