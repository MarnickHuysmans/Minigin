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

		void SetPosition(float x, float y);
		const Transform& GetTransform() const;

		void AddComponent(const std::shared_ptr<Component>& component);
		void AddComponent(const std::shared_ptr<UIComponent>& uiComponent);

		template<typename ComponentType>
		std::weak_ptr<ComponentType> GetComponent();
		template<typename ComponentType>
		std::vector<std::weak_ptr<ComponentType>> GetComponents();

		const std::vector<std::shared_ptr<UIComponent>> GetUIComponents() const;

		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform{};
		std::vector<std::shared_ptr<Component>> m_Components{};
		std::vector<std::shared_ptr<UIComponent>> m_UiComponents{};
	};

	template <typename ComponentType>
	std::weak_ptr<ComponentType> GameObject::GetComponent()
	{
		static_assert(std::is_base_of<Component, ComponentType>().value, "The type should be derived from Component");
		for (auto component : m_Components)
		{
			std::shared_ptr<ComponentType> found = std::dynamic_pointer_cast<ComponentType>(component);
			if (found)
			{
				return std::weak_ptr<ComponentType>{found};
			}
		}
		return std::weak_ptr<ComponentType>{};
	}

	template <typename ComponentType>
	std::vector<std::weak_ptr<ComponentType>> GameObject::GetComponents()
	{
		static_assert(std::is_base_of<Component, ComponentType>().value, "The type should be derived from Component");
		std::vector<std::weak_ptr<ComponentType>> components{};
		for (auto component : m_Components)
		{
			std::shared_ptr<ComponentType> found = std::dynamic_pointer_cast<ComponentType>(component);
			if (found)
			{
				components.push_back(found);
			}
		}
		return components;
	}
}
