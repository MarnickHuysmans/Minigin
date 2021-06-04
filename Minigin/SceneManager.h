#pragma once
#include <functional>
#include <vector>

#include "Singleton.h"

#include <string>
#include <memory>

namespace dae
{
	class Scene;
	class RenderComponent;
	class UIComponent;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		void CreateScene(const std::string& name, const std::function<void(Scene&)>& sceneFunction);
		std::weak_ptr<Scene> GetCurrentScene() const;
		void SwitchScene(const std::string& name);
		void SwitchScene(size_t index);

		void Start();
		void Update();
		void Render() const;
		void RenderUi() const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<std::pair<std::string, std::function<void(Scene&)>>> m_Scenes{};
		std::shared_ptr<Scene> m_CurrentScene;
		size_t m_SwitchIndex = 0;
		bool m_Switch = false;
	};
}
