#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class RenderComponent;
	class UIComponent;
	class Scene final
	{
	public:
		void Add(const std::shared_ptr<GameObject>& object);
		void Remove(const std::weak_ptr<GameObject>& object);

		void Start();
		void Update();
		void Render();
		void RenderUi();

		const std::string& GetName() const;
		void Sort();

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		Scene(const std::string& name);

		void AddRenderingComponents(const std::shared_ptr<GameObject>& object);
		void SortRenderComponents();

		std::string m_Name{};
		std::vector < std::shared_ptr<GameObject>> m_Objects{};
		std::vector<std::weak_ptr<RenderComponent>> m_RenderComponents{};
		std::vector<std::weak_ptr<UIComponent>> m_UiComponents{};
		std::vector<std::weak_ptr<GameObject>> m_ObjectsToDelete{};

		bool m_Started;
		bool m_Sort;

		friend class GameObject;
		friend void SceneManager::Start();
	};

}
