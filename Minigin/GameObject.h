#pragma once
#include "Component.h"
#include "Transform.h"
#include "UIComponent.h"

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	public:
		void Start();
		void Update();
		void Render() const;
		void RenderUi();

		void SetPosition(float x, float y);
		const Transform& GetTransform() const;

		void AddComponent(Component* component);
		void AddComponent(const std::shared_ptr<UIComponent>& uiComponent);

		template<typename ComponentType>
		ComponentType* GetComponent();

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform{};
		std::vector<Component*> m_Components{};
		std::vector<std::shared_ptr<UIComponent>> m_UiComponents{};
	};

	template <typename ComponentType>
	ComponentType* GameObject::GetComponent()
	{
		static_assert(std::is_base_of<Component, ComponentType>().value, "The type should be derived from Component");
		for (auto component : m_Components)
		{
			ComponentType* found = dynamic_cast<ComponentType*>(component);
			if (found)
			{
				return found;
			}
		}
		return nullptr;
	}
}
