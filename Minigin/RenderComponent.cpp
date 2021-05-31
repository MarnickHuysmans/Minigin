#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

void dae::RenderComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		const auto& pos = m_GameObject->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
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

dae::RenderComponent::RenderComponent(const std::string& filename)
{
	SetTexture(filename);
}
