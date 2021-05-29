#pragma once
#include <Component.h>
#include <memory>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

namespace qbert
{
	class Walkable : public dae::Component
	{
	public:
		Walkable() = default;
		virtual ~Walkable();
		
		void SetTopMiddle(const glm::vec2& offset) { m_TopMiddle = offset; }
		void SetRightMiddle(const glm::vec2& offset) { m_RightMiddle = offset; }
		void SetLeftMiddle(const glm::vec2& offset) { m_LeftMiddle = offset; }

		void SetTopUp(std::weak_ptr<Walkable>& walkable) { m_TopUp = walkable; }
		void SetTopRight(std::weak_ptr<Walkable>& walkable) { m_TopRight = walkable; }
		void SetTopDown(std::weak_ptr<Walkable>& walkable) { m_TopDown = walkable; }
		void SetTopLeft(std::weak_ptr<Walkable>& walkable) { m_TopLeft = walkable; }

		void SetRightUp(std::weak_ptr<Walkable>& walkable) { m_RightUp = walkable; }
		void SetRightLeft(std::weak_ptr<Walkable>& walkable) { m_RightLeft = walkable; }

		void SetLeftUp(std::weak_ptr<Walkable>& walkable) { m_LeftUp = walkable; }
		void SetLeftRight(std::weak_ptr<Walkable>& walkable) { m_LeftRight = walkable; }

		const glm::vec2& GetTopMiddle() const { return m_TopMiddle; }
		const glm::vec2& GetRightMiddle() const { return m_RightMiddle; }
		const glm::vec2& GetLeftMiddle() const { return m_LeftMiddle; }

		const std::weak_ptr<Walkable>& GetTopUp() const { return m_TopUp; }
		const std::weak_ptr<Walkable>& GetTopRight() const { return m_TopRight; }
		const std::weak_ptr<Walkable>& GetTopDown() const { return m_TopDown; }
		const std::weak_ptr<Walkable>& GetTopLeft() const { return m_TopLeft; }

		const std::weak_ptr<Walkable>& GetRightUp() const { return m_RightUp; }
		const std::weak_ptr<Walkable>& GetRightLeft() const { return m_RightLeft; }
		const std::weak_ptr<Walkable>& GetLeftUp() const { return m_LeftUp; } 
		const std::weak_ptr<Walkable>& GetLeftRight() const { return m_LeftRight; }

	protected:
		glm::vec2 m_TopMiddle;
		glm::vec2 m_RightMiddle;
		glm::vec2 m_LeftMiddle;

		std::weak_ptr<Walkable> m_TopUp;
		std::weak_ptr<Walkable> m_TopRight;
		std::weak_ptr<Walkable> m_TopDown;
		std::weak_ptr<Walkable> m_TopLeft;

		std::weak_ptr<Walkable> m_RightUp;
		std::weak_ptr<Walkable> m_RightLeft;

		std::weak_ptr<Walkable> m_LeftUp;
		std::weak_ptr<Walkable> m_LeftRight;
	};
}
