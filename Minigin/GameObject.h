#pragma once
#include "Component.h"
#include "Transform.h"
#include "UIComponent.h"

#include <memory>
#include <vector>

namespace dae
{
	class Scene;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		void Start();
		void Update();

		Transform& GetTransform();
		const Transform& GetTransform() const;

		void AddComponent(const std::shared_ptr<Component>& component);
		void AddComponent(const std::shared_ptr<UIComponent>& uiComponent);
		void RemoveComponent(const std::weak_ptr<Component>& component);
		void RemoveComponent(const std::weak_ptr<UIComponent>& uiComponent);

		void AddChild(const std::shared_ptr<GameObject>& child);
		void Destroy();
		const std::vector<std::shared_ptr<GameObject>>& GetChildren() const;

		template<typename ComponentType>
		std::weak_ptr<ComponentType> GetComponent();
		template<typename ComponentType>
		std::vector<std::weak_ptr<ComponentType>> GetComponents(bool children = true);

		std::vector<std::shared_ptr<UIComponent>> GetUIComponents(bool children = true) const;

		Scene* GetScene();
		const Scene* GetScene() const;
		GameObject* GetParent();
		const GameObject* GetParent() const;

		GameObject();
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void RemoveChild(const std::weak_ptr<GameObject>& child);
		
		Transform m_Transform;
		Scene* m_Scene;
		GameObject* m_Parent;
		std::vector<std::shared_ptr<Component>> m_Components{};
		std::vector<std::shared_ptr<UIComponent>> m_UiComponents{};
		std::vector<std::shared_ptr<GameObject>> m_Children{};
		bool m_Started;

		friend class Scene;
	};

	template <typename ComponentType>
	std::weak_ptr<ComponentType> GameObject::GetComponent()
	{
		static_assert(std::is_base_of<Component, ComponentType>().value, "The type should be derived from Component");
		for (auto& component : m_Components)
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
	std::vector<std::weak_ptr<ComponentType>> GameObject::GetComponents(bool children)
	{
		static_assert(std::is_base_of<Component, ComponentType>().value, "The type should be derived from Component");
		std::vector<std::weak_ptr<ComponentType>> components{};
		for (auto& component : m_Components)
		{
			std::shared_ptr<ComponentType> found = std::dynamic_pointer_cast<ComponentType>(component);
			if (found)
			{
				components.push_back(found);
			}
		}
		if (!children)
		{
			return components;
		}
		for (auto& child : m_Children)
		{
			std::vector<std::weak_ptr<ComponentType>> childComponents = child->GetComponents<ComponentType>(true);
			components.insert(std::end(components), std::begin(childComponents), std::end(childComponents));
		}
		return components;
	}
}
