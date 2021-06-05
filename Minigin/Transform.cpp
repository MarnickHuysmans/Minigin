#include "MiniginPCH.h"
#include "Transform.h"
#include "GameObject.h"
#include "Scene.h"

dae::Transform::Transform(GameObject* gameObject) :
	m_GameObject(gameObject),
	m_LocalPosition(0, 0, 0),
	m_WorldPosition(0, 0, 0),
	m_LocalScale(1, 1),
	m_WorldScale(1, 1)
{
}

void dae::Transform::SetWorldPosition(const float x, const float y, const float z)
{
	SetWorldPosition(glm::vec3(x, y, z));
}

void dae::Transform::SetWorldPosition(const glm::vec3& position)
{
	if (m_WorldPosition == position)
	{
		return;
	}
	float previousZ = m_WorldPosition.z;
	m_WorldPosition = position;
	m_LocalPosition = LocalPosition();
	UpdateChildrenPosition();
	SortCheck(previousZ);
}

void dae::Transform::SetLocalPosition(float x, float y, float z)
{
	SetLocalPosition(glm::vec3(x, y, z));
}

void dae::Transform::SetLocalPosition(const glm::vec3& position)
{
	if (m_LocalPosition == position)
	{
		return;
	}
	float previousZ = m_WorldPosition.z;
	m_LocalPosition = position;
	m_WorldPosition = WorldPosition();
	UpdateChildrenPosition();
	SortCheck(previousZ);
}

void dae::Transform::SetWorldScale(float x, float y)
{
	SetWorldScale(glm::vec2(x, y));
}

void dae::Transform::SetWorldScale(const glm::vec2& scale)
{
	if (m_WorldScale == scale)
	{
		return;
	}
	m_WorldScale = scale;
	m_LocalScale = LocalScale();
	UpdateChildrenScale();
}

void dae::Transform::SetLocalScale(float x, float y)
{
	SetLocalScale(glm::vec2(x, y));
}

void dae::Transform::SetLocalScale(const glm::vec2& scale)
{
	if (m_LocalScale == scale)
	{
		return;
	}
	m_LocalScale = scale;
	m_WorldScale = WorldScale();
	UpdateChildrenScale();
}

void dae::Transform::UpdatePosition()
{
	m_WorldPosition = WorldPosition();
	UpdateChildrenPosition();
}

void dae::Transform::UpdateChildrenPosition()
{
	if (m_GameObject)
	{
		auto& children = m_GameObject->GetChildren();
		for (auto& gameObject : children)
		{
			gameObject->GetTransform().UpdatePosition();
		}
	}
}

void dae::Transform::UpdateScale()
{
	m_WorldScale = WorldScale();
	m_WorldPosition = WorldPosition();
	UpdateChildrenScale();
}

void dae::Transform::UpdateChildrenScale()
{
	if (m_GameObject)
	{
		auto& children = m_GameObject->GetChildren();
		for (auto& gameObject : children)
		{
			gameObject->GetTransform().UpdateScale();
		}
	}
}

void dae::Transform::UpdateTransform()
{
	m_WorldScale = WorldScale();
	m_WorldPosition = WorldPosition();
	UpdateChildrenTransform();
}

void dae::Transform::UpdateChildrenTransform()
{
	if (m_GameObject)
	{
		auto& children = m_GameObject->GetChildren();
		for (auto& gameObject : children)
		{
			gameObject->GetTransform().UpdateTransform();
		}
	}
}

glm::vec3 dae::Transform::WorldPosition() const
{
	if (m_GameObject == nullptr || m_GameObject->GetParent() == nullptr)
	{
		return m_LocalPosition;
	}
	const auto& parentPosition = m_GameObject->GetParent()->GetTransform().GetWorldPosition();
	const auto& parentScale = m_GameObject->GetParent()->GetTransform().GetWorldScale();
	return parentPosition + m_LocalPosition * glm::vec3(parentScale.x, parentScale.y, 1);
}

glm::vec2 dae::Transform::WorldScale() const
{
	if (m_GameObject == nullptr || m_GameObject->GetParent() == nullptr)
	{
		return m_LocalScale;
	}
	const auto& parentScale = m_GameObject->GetParent()->GetTransform().GetWorldScale();
	return parentScale * m_LocalScale;
}

glm::vec3 dae::Transform::LocalPosition() const
{
	if (m_GameObject == nullptr || m_GameObject->GetParent() == nullptr)
	{
		return m_WorldPosition;
	}
	const auto& parentPosition = m_GameObject->GetParent()->GetTransform().GetWorldPosition();
	const auto& parentScale = m_GameObject->GetParent()->GetTransform().GetWorldScale();
	return (m_WorldPosition - parentPosition) / glm::vec3(parentScale.x, parentScale.y, 1);
}

glm::vec2 dae::Transform::LocalScale() const
{
	if (m_GameObject == nullptr || m_GameObject->GetParent() == nullptr)
	{
		return m_WorldScale;
	}
	const auto& parentScale = m_GameObject->GetParent()->GetTransform().GetWorldScale();
	return m_WorldScale / parentScale;
}

void dae::Transform::SortCheck(float previousZ) const
{
	if (previousZ != m_WorldPosition.z)
	{
		auto* scene = m_GameObject->GetScene();
		if (scene != nullptr)
		{
			scene->Sort();
		}
	}
}
