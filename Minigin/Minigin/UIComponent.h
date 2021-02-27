#pragma once
#include <functional>
#include "imgui.h"

namespace dae
{
	class UIComponent
	{
	public:
		void Render();

		UIComponent(const std::function<void()>& function, const std::string& name = "UI", bool* open = nullptr, ImGuiWindowFlags flags = 0);
		virtual ~UIComponent() = default;
		UIComponent(const UIComponent& other) = delete;
		UIComponent(UIComponent&& other) noexcept = delete;
		UIComponent& operator=(const UIComponent& other) = delete;
		UIComponent& operator=(UIComponent&& other) noexcept = delete;
		
	protected:
		
		std::string m_Name{};
		bool* m_Open = nullptr;
		ImGuiWindowFlags m_Flags = 0;
		std::function<void()> m_Function{};
	};
}
