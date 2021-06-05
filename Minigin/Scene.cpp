#include "MiniginPCH.h"
#include "Scene.h"
#include <algorithm>
#include "GameObject.h"
#include "RenderComponent.h"

using namespace dae;

Scene::Scene(const std::string& name) :
	m_Name(name),
	m_Started(false),
	m_Sort(false)
{
}


void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	auto parentScene = object->GetScene();
	if (parentScene != nullptr && parentScene != this)
	{
		return;
	}
	if (object->m_Scene != nullptr)
	{
		return;
	}
	m_Objects.push_back(object);
	if (object->m_Parent != nullptr)
	{
		object->m_Parent->RemoveChild(object);
	}
	object->m_Scene = this;
	object->GetTransform().UpdateTransform();
	if (m_Started)
	{
		object->Start();
	}
}

void Scene::Remove(const std::weak_ptr<GameObject>& object)
{
	if (object.expired())
	{
		return;
	}
	auto gameObject = object.lock();
	gameObject->m_Scene = nullptr;
	m_Objects.erase(std::remove(std::begin(m_Objects), std::end(m_Objects), gameObject), std::end(m_Objects));
}

void Scene::Start()
{
	for (auto& gameObject : m_Objects)
	{
		gameObject->Start();
	}
	m_Started = true;
}

void Scene::Update()
{
	for (auto& object : m_ObjectsToDelete)
	{
		if (object.expired())
		{
			continue;
		}
		auto gameObject = object.lock();
		if (gameObject->m_Scene != nullptr)
		{
			Remove(object);
			continue;
		}
		auto parent = gameObject->m_Parent;
		if (parent != nullptr)
		{
			parent->RemoveChild(object);
		}
	}
	m_ObjectsToDelete.clear();
	for (auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::Render()
{
	if (m_Sort)
	{
		SortRenderComponents();
	}
	m_RenderComponents.erase(std::remove_if(std::begin(m_RenderComponents), std::end(m_RenderComponents), [](std::weak_ptr<RenderComponent>& renderComponent)
		{
			if (renderComponent.expired())
			{
				return true;
			}
			renderComponent.lock()->Render();
			return false;
		}), std::end(m_RenderComponents));
}

void Scene::RenderUi()
{
	m_UiComponents.erase(std::remove_if(std::begin(m_UiComponents), std::end(m_UiComponents), [](std::weak_ptr<UIComponent>& uiComponent)
		{
			if (uiComponent.expired())
			{
				return true;
			}
			uiComponent.lock()->Render();
			return false;
		}), std::end(m_UiComponents));
}

const std::string& Scene::GetName() const
{
	return m_Name;
}

void Scene::Sort()
{
	if (!m_Sort)
	{
		m_Sort = true;
	}
}

void Scene::AddRenderingComponents(const std::shared_ptr<GameObject>& object)
{
	for (auto& component : object->GetUIComponents(false))
	{
		if (component->m_InScene)
		{
			continue;
		}
		m_UiComponents.push_back(component);
		component->m_InScene = true;
	}

	bool newRenderComponents = false;

	for (auto& component : object->GetComponents<RenderComponent>(false))
	{
		if (component.expired())
		{
			continue;
		}
		auto renderComponent = component.lock();
		if (renderComponent->m_InScene)
		{
			continue;
		}
		m_RenderComponents.push_back(component);
		renderComponent->m_InScene = true;
		newRenderComponents = true;
	}
	if (newRenderComponents)
	{
		Sort();
	}
}

void Scene::SortRenderComponents()
{
	std::sort(std::begin(m_RenderComponents), std::end(m_RenderComponents), [](std::weak_ptr<RenderComponent>& renderComponent1, std::weak_ptr<RenderComponent>& renderComponent2)
		{
			if (renderComponent1.expired())
			{
				return false;
			}
			if (renderComponent2.expired())
			{
				return true;
			}
			return renderComponent1.lock()->GetGameObject()->GetTransform().GetWorldPosition().z < renderComponent2.lock()->GetGameObject()->GetTransform().GetWorldPosition().z;
		});
	m_Sort = false;
}
