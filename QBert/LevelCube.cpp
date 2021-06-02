#include "LevelCube.h"

#include "GameObject.h"
#include "LevelCubeActivator.h"
#include "LevelEnums.h"
#include "Movement.h"

qbert::LevelCube::LevelCube(LevelType type, const std::weak_ptr<Level>& level, int row, int col, const glm::vec3& topMiddle, const glm::vec3& rightMiddle, const glm::vec3& leftMiddle) :
	Walkable(level, row, col, topMiddle, rightMiddle, leftMiddle),
	m_State(0),
	m_MaxStates(MaxStates(type)),
	m_Textures(3),
	m_Type(type)
{
}

void qbert::LevelCube::SetTexture(std::shared_ptr<dae::Texture2D>& texture, size_t index)
{
	if (index >= m_MaxStates)
	{
		return;
	}
	m_Textures[index] = texture;
}

void qbert::LevelCube::Start()
{
	m_RenderComponent = m_GameObject->GetComponent<dae::RenderComponent>();
	SetCurrentTexture();
}

qbert::Walkable::WalkableType qbert::LevelCube::GetWalkableType()
{
	return WalkableType::LevelCube;
}

void qbert::LevelCube::StepOn(qbert::Movement* movement)
{
	if (movement == nullptr)
	{
		return;
	}
	
	auto activator = movement->GetGameObject()->GetComponent<LevelCubeActivator>();
	if (activator.expired())
	{
		return;
	}
	StepOn(activator.lock()->GetForward());
}

void qbert::LevelCube::StepOn(bool forward)
{
	switch (m_Type)
	{
	case LevelType::Single:
		StepOnStandard(forward);
		break;
	case LevelType::Double:
		StepOnStandard(forward);
		break;
	case LevelType::Cycle:
		StepOnCycle(forward);
		break;
	}
}

size_t qbert::LevelCube::MaxStates(LevelType type) const
{
	switch (type)
	{
	case LevelType::Single:
		return  2;
	case LevelType::Double:
		return  3;
	case LevelType::Cycle:
		return  2;
	default:
		return  1;
	}
}

void qbert::LevelCube::SetCurrentTexture()
{
	if (m_RenderComponent.expired())
	{
		return;
	}
	m_RenderComponent.lock()->SetTexture(m_Textures[m_State]);
}

void qbert::LevelCube::StepOnStandard(bool forward)
{
	if (forward)
	{
		if (m_State >= m_MaxStates - 1)
		{
			return;
		}
		++m_State;
		SetCurrentTexture();
		return;
	}
	if (m_State == 0)
	{
		return;
	}
	--m_State;
	SetCurrentTexture();
}

void qbert::LevelCube::StepOnCycle(bool forward)
{
	m_State += forward ? 1 : -1;
	if (forward)
	{
		m_State %= m_MaxStates;
	}
	else if (m_State >= m_MaxStates)
	{
		m_State = m_MaxStates - 1;
	}
	SetCurrentTexture();
}
