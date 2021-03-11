#pragma once
namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual void Start() {};
		virtual void Update() = 0;

		const GameObject& GetGameObject() const;

		Component() = default;
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) noexcept = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept = delete;
	protected:
		friend class GameObject;
		GameObject* m_GameObject = nullptr;
	};
}
