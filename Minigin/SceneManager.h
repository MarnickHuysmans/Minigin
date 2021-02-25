#pragma once
#include "Singleton.h"

namespace dae
{
	class Scene;
	class RenderComponent;
	class UIComponent;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Start();
		void Update();
		void Render() const;
		void RenderUi();
		
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		int m_CurrentScene{-1};
	};
}
