#include "MiniginPCH.h"
#include "UIComponent.h"

void dae::UIComponent::Render()
{
	ImGui::SetNextWindowPos(m_Position);
	if (ImGui::Begin(m_Name.c_str(), m_Open, m_Flags))
	{
		m_Function();
	}
	ImGui::End();
}

void dae::UIComponent::SetPosition(const ImVec2& position)
{
	m_Position = position;
}

dae::UIComponent::UIComponent(const std::function<void()>& function, const std::string& name, bool* open, ImGuiWindowFlags flags)
	:m_Name(name),
	m_Open(open),
	m_Flags(flags),
	m_Function(function)
{

}
