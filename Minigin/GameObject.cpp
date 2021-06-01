#include "MiniginPCH.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Scene.h"

void dae::GameObject::Start()
{
	if (m_Started)
	{
		return;
	}
	for (auto& component : m_Components)
	{
		component->Start();
	}
	for (auto& child : m_Children)
	{
		child->Start();
	}
	m_Started = true;
}

void dae::GameObject::Update()
{
	for (auto& component : m_Components)
	{
		component->Update();
	}
	for (auto& child : m_Children)
	{
		child->Update();
	}
}

dae::Transform& dae::GameObject::GetTransform()
{
	return m_Transform;
}

const dae::Transform& dae::GameObject::GetTransform() const
{
	return m_Transform;
}

void dae::GameObject::AddComponent(const std::shared_ptr<Component>& component)
{
	if (component->m_GameObject != nullptr)
	{
		return;
	}
	m_Components.push_back(component);
	component->m_GameObject = this;
	if (m_Started)
	{
		component->Start();
	}
}

void dae::GameObject::AddComponent(const std::shared_ptr<UIComponent>& uiComponent)
{
	m_UiComponents.push_back(uiComponent);
}

void dae::GameObject::RemoveComponent(const std::weak_ptr<Component>& component)
{
	if (component.expired())
	{
		return;
	}
	auto localComponent = component.lock();
	localComponent->m_GameObject = nullptr;
	m_Components.erase(std::remove(std::begin(m_Components), std::end(m_Components), localComponent), std::end(m_Components));
}

void dae::GameObject::RemoveComponent(const std::weak_ptr<UIComponent>& uiComponent)
{
	if (uiComponent.expired())
	{
		return;
	}
	m_UiComponents.erase(std::remove(std::begin(m_UiComponents), std::end(m_UiComponents), uiComponent.lock()), std::end(m_UiComponents));
}

void dae::GameObject::AddChild(const std::shared_ptr<GameObject>& child)
{
	auto childScene = child->GetScene();
	if (childScene != nullptr && childScene != GetScene())
	{
		return;
	}
	m_Children.push_back(child);
	if (child->m_Parent != nullptr)
	{
		child->m_Parent->RemoveChild(child);
	}
	if (child->m_Scene != nullptr)
	{
		child->m_Scene->Remove(child);
	}
	child->m_Parent = this;
	child->GetTransform().UpdateTransform();
	if (m_Started && !child->m_Started)
	{
		GetScene()->AddRenderingComponents(child);
		child->Start();
	}
}

void dae::GameObject::RemoveChild(const std::weak_ptr<GameObject>& child)
{
	if (child.expired())
	{
		return;
	}
	auto gameObject = child.lock();
	gameObject->m_Parent = nullptr;
	m_Children.erase(std::remove(std::begin(m_Children), std::end(m_Children), gameObject), std::end(m_Children));
}

const std::vector<std::shared_ptr<dae::GameObject>>& dae::GameObject::GetChildren() const
{
	return m_Children;
}

std::vector<std::shared_ptr<dae::UIComponent>> dae::GameObject::GetUIComponents(bool children) const
{
	if (!children)
	{
		return m_UiComponents;
	}
	std::vector<std::shared_ptr<UIComponent>> uiComponents(m_UiComponents);
	for (auto child : GetChildren())
	{
		std::vector<std::shared_ptr<UIComponent>> childComponents = child->GetUIComponents(children);
		std::move(std::begin(childComponents), std::end(childComponents), std::back_inserter(uiComponents));
	}
	return uiComponents;
}

dae::Scene* dae::GameObject::GetScene()
{
	if (m_Scene != nullptr)
	{
		return m_Scene;
	}
	if (m_Parent != nullptr)
	{
		return m_Parent->GetScene();
	}
	return nullptr;
}

const dae::Scene* dae::GameObject::GetScene() const
{
	if (m_Scene != nullptr)
	{
		return m_Scene;
	}
	if (m_Parent != nullptr)
	{
		return m_Parent->GetScene();
	}
	return nullptr;
}

dae::GameObject* dae::GameObject::GetParent()
{
	return m_Parent;
}

const dae::GameObject* dae::GameObject::GetParent() const
{
	return m_Parent;
}

dae::GameObject::GameObject() :
	m_Transform(this),
	m_Scene(nullptr),
	m_Parent(nullptr),
	m_Started(false)
{
}
