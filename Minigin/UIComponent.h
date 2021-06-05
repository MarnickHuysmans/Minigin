#pragma once
#include <functional>
#include "imgui.h"

#include <string>

namespace dae
{
	class UIComponent final
	{
	public:
		void Render();
		void SetPosition(const ImVec2& position);
		void SetPivot(const ImVec2& pivot);

		void SetOpen(bool* open) { m_Open = open; }

		bool IsActive() const { return  m_Active; }
		void SetActive(bool active) { m_Active = active; }

		UIComponent(const std::function<void()>& function, const std::string& name = "UI", bool* open = nullptr, ImGuiWindowFlags flags = 0);
		~UIComponent() = default;
		UIComponent(const UIComponent& other) = delete;
		UIComponent(UIComponent&& other) noexcept = delete;
		UIComponent& operator=(const UIComponent& other) = delete;
		UIComponent& operator=(UIComponent&& other) noexcept = delete;

	protected:

		std::string m_Name{};
		bool* m_Open = nullptr;
		ImGuiWindowFlags m_Flags = 0;
		std::function<void()> m_Function{};
		ImVec2 m_Position{};
		ImVec2 m_Pivot{};

	private:
		bool m_InScene;
		bool m_Active;

		friend class Scene;
	};
}
