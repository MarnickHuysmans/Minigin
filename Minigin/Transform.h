#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

namespace dae
{
	class GameObject;

	class Transform final
	{
	public:
		Transform(GameObject* gameObject);
		~Transform() = default;

		const glm::vec3& GetWorldPosition() const { return m_WorldPosition; }
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		void SetWorldPosition(float x, float y, float z);
		void SetWorldPosition(const glm::vec3& position);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(const glm::vec3& position);

		const glm::vec2& GetWorldScale() const { return m_WorldScale; }
		const glm::vec2& GetLocalScale() const { return m_LocalScale; }
		void SetWorldScale(float x, float y);
		void SetWorldScale(const glm::vec2& scale);
		void SetLocalScale(float x, float y);
		void SetLocalScale(const glm::vec2& scale);

	private:
		void UpdatePosition();
		void UpdateChildrenPosition();
		void UpdateScale();
		void UpdateChildrenScale();

		void UpdateTransform();
		void UpdateChildrenTransform();

		glm::vec3 WorldPosition() const;
		glm::vec2 WorldScale() const;

		glm::vec3 LocalPosition() const;
		glm::vec2 LocalScale() const;

		void SortCheck(float previousZ) const;

		GameObject* m_GameObject;

		glm::vec3 m_LocalPosition;
		glm::vec3 m_WorldPosition;

		glm::vec2 m_LocalScale;
		glm::vec2 m_WorldScale;

		friend class GameObject;
		friend class Scene;
	};
}
