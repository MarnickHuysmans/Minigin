#include "MiniginPCH.h"
#include "GameObject.h"
#include "RenderComponent.h"

void dae::GameObject::Start()
{
	for (auto& component : m_Components)
	{
		component->Start();
	}
}

void dae::GameObject::Update()
{
	for (auto& component : m_Components)
	{
		component->Update();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
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
	m_Components.push_back(component);
	component->m_GameObject = this;
}

void dae::GameObject::AddComponent(const std::shared_ptr<UIComponent>& uiComponent)
{
	m_UiComponents.push_back(uiComponent);
}

std::vector<std::shared_ptr<dae::UIComponent>> dae::GameObject::GetUIComponents() const
{
	return m_UiComponents;
}
