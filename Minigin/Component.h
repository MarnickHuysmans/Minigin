#pragma once
#include <memory>

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual void Start() {}
		virtual void Update() {}

		std::shared_ptr<GameObject> GetGameObject() const;

		Component() = default;
		virtual ~Component() = 0;
		Component(const Component& other) = delete;
		Component(Component&& other) noexcept = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept = delete;
	protected:
		friend class GameObject;
		GameObject* m_GameObject = nullptr;
	};
}
