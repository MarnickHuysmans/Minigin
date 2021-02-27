#include "MiniginPCH.h"
#include "UIComponent.h"

void dae::UIComponent::Render()
{
	if (ImGui::Begin(m_Name.c_str(), m_Open, m_Flags))
	{
		m_Function();
	}
	ImGui::End();
}

dae::UIComponent::UIComponent(const std::function<void()>& function, const std::string& name, bool* open, ImGuiWindowFlags flags)
	:m_Name(name),
	m_Open(open),
	m_Flags(flags),
	m_Function(function)
{

}
