#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::CreateScene(const std::string& name, const std::function<void(Scene&)>& sceneFunction)
{
	auto it = std::find_if(std::begin(m_Scenes), std::end(m_Scenes), [&name](const std::pair<std::string, std::function<void(Scene&)>>& scene)
		{
			return scene.first == name;
		});
	if (it != std::end(m_Scenes))
	{
		return;
	}
	m_Scenes.push_back({ name, sceneFunction });
}

std::weak_ptr<dae::Scene> dae::SceneManager::GetCurrentScene() const
{
	return m_CurrentScene;
}

void dae::SceneManager::SwitchScene(const std::string& name)
{
	auto it = std::find_if(std::begin(m_Scenes), std::end(m_Scenes), [&name](const std::pair<std::string, std::function<void(Scene&)>>& scene)
		{
			return scene.first == name;
		});
	if (it == std::end(m_Scenes))
	{
		return;
	}
	m_SwitchIndex = std::distance(std::begin(m_Scenes), it);
	m_Switch = true;
}

void dae::SceneManager::SwitchScene(size_t index)
{
	if (index >= m_Scenes.size())
	{
		return;
	}
	m_SwitchIndex = index;
	m_Switch = true;
}

void dae::SceneManager::Start()
{
	if (m_Scenes.empty())
	{
		m_CurrentScene = std::shared_ptr<Scene>(new Scene("EmptyScene"));
	}
	else
	{
		auto& startScene = m_Scenes[m_SwitchIndex];
		m_CurrentScene = std::shared_ptr<Scene>(new Scene(startScene.first));
		startScene.second(*m_CurrentScene);
	}
	m_CurrentScene->Start();
}

void dae::SceneManager::Update()
{
	if (m_Switch)
	{
		Start();
		m_Switch = false;
	}
	m_CurrentScene->Update();
}

void dae::SceneManager::Render() const
{
	m_CurrentScene->Render();
}

void dae::SceneManager::RenderUi() const
{
	m_CurrentScene->RenderUi();
}
