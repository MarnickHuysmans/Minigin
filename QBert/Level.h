#pragma once
#include <Component.h>
#include <memory>

#include "Walkable.h"

namespace qbert
{
	class Level final : public dae::Component
	{
	public:
		Level() = default;
		~Level() = default;

		void SetTopCube(std::weak_ptr<Walkable>& cube) { m_TopCube = cube; }
		void SetBottomRightCube(std::weak_ptr<Walkable>& cube) { m_BottomRightCube = cube; }
		void SetBottomLeftCube(std::weak_ptr<Walkable>& cube) { m_BottomLeftCube = cube; }

		const std::weak_ptr<Walkable>& GetTopCube() const { return m_TopCube; }
		const std::weak_ptr<Walkable>& GetTopRightCube();
		const std::weak_ptr<Walkable>& GetTopLeftCube();
		const std::weak_ptr<Walkable>& GetBottomRightCube() const { return m_BottomRightCube; }
		const std::weak_ptr<Walkable>& GetBottomLeftCube() const { return m_BottomLeftCube; }

	private:
		std::weak_ptr<Walkable> m_TopCube;
		std::weak_ptr<Walkable> m_BottomRightCube;
		std::weak_ptr<Walkable> m_BottomLeftCube;
	};
}
