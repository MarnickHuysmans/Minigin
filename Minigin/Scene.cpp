#include "MiniginPCH.h"
#include "Scene.h"
#include <algorithm>
#include "GameObject.h"
#include "RenderComponent.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>&object)
{
	m_Objects.push_back(object);
	AddRenderingComponents(object);
}

void Scene::Start()
{
	for (auto gameObject : m_Objects)
	{
		gameObject->Start();
	}
}

void Scene::Update()
{
	for (auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::Render()
{
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

void Scene::AddRenderingComponents(const std::shared_ptr<GameObject>&object)
{
	for (auto& component : object->GetUIComponents())
	{
		m_UiComponents.push_back(component);
	}

	for (auto& component : object->GetComponents<RenderComponent>())
	{
		m_RenderComponents.push_back(component);
	}
}
