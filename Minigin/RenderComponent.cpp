#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

void dae::RenderComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		const auto& pos = m_GameObject->GetTransform().GetWorldPosition();
		const auto& scale = m_GameObject->GetTransform().GetWorldScale();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, scale.x, scale.y);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	if (!filename.empty())
	{
		m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
	}
}

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D>& texture)
{
	m_Texture = texture;
}

dae::RenderComponent::RenderComponent(const std::string& filename) :
	m_InScene(false)
{
	SetTexture(filename);
}
