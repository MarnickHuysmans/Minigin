#include "MiniginPCH.h"
#include "GameObject.h"
#include "RenderComponent.h"

dae::GameObject::~GameObject()
{
	for (auto component : m_Components)
	{
		delete component;
	}
};

void dae::GameObject::Start()
{
	for (auto component : m_Components)
	{
		component->Start();
	}
}

void dae::GameObject::Update()
{
	for (auto component : m_Components)
	{
		component->Update();
	}
}

void dae::GameObject::Render() const
{
	for (auto component : m_Components)
	{
		component->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

const dae::Transform& dae::GameObject::GetTransform() const
{
	return m_Transform;
}

void dae::GameObject::AddComponent(Component* component)
{
	m_Components.push_back(component);
	component->m_GameObject = this;
}
