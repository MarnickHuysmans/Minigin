#pragma once
#include <memory>

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		void RootUpdate();
		virtual void Start() {}
		virtual void Update() {}

		std::weak_ptr<GameObject> GetGameObject() const;

		bool IsActive() const { return m_Active; }
		void SetActive(bool active) { m_Active = active; }
		bool ActiveInScene() const;

		Component() = default;
		virtual ~Component() = 0;
		Component(const Component& other) = delete;
		Component(Component&& other) noexcept = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept = delete;
	protected:
		friend class GameObject;
		std::weak_ptr<GameObject> m_GameObject;
		bool m_Active = true;
	};
}
