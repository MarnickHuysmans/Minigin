#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Start()
{
	for (auto& scene : m_Scenes)
	{
		scene->Start();
	}
}

void dae::SceneManager::Update()
{
	m_Scenes[m_CurrentScene]->Update();
}

void dae::SceneManager::Render() const
{
	m_Scenes[m_CurrentScene]->Render();
}

void dae::SceneManager::RenderUi()
{
	m_Scenes[m_CurrentScene]->RenderUi();
}

dae::Scene* dae::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_CurrentScene = m_Scenes.size();
	m_Scenes.push_back(scene);
	return scene.get();
}

dae::Scene* dae::SceneManager::GetScene(const std::string& name)
{
	auto it = std::find_if(std::begin(m_Scenes), std::end(m_Scenes), [&name](std::shared_ptr<Scene>& scene)
		{
			return scene->GetName() == name;
		});
	if (it != std::end(m_Scenes))
	{
		return it->get();
	}
	return nullptr;
}
