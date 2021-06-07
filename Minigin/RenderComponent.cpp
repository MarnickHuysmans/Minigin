#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

void dae::RenderComponent::Render() const
{
	if (!ActiveInScene() || m_Texture == nullptr)
	{
		return;
	}
	if (m_GameObject.expired())
	{
		return;
	}
	auto gameObject = m_GameObject.lock();
	const auto& pos = gameObject->GetTransform().GetWorldPosition();
	const auto& scale = gameObject->GetTransform().GetWorldScale();
	const auto& pivot = gameObject->GetTransform().GetPivot();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, scale.x, scale.y, pivot.x, pivot.y);
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	if (!filename.empty())
	{
		m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
	}
}

void dae::RenderComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
	m_Texture = texture;
}

dae::RenderComponent::RenderComponent(const std::string& filename) :
	m_InScene(false)
{
	SetTexture(filename);
}
