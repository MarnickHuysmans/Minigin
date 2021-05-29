#include "LevelCube.h"
#include "LevelType.h"

qbert::LevelCube::LevelCube(LevelType type) :
	m_State(0),
	m_MaxStates(MaxStates(type)),
	m_Textures(m_MaxStates),
	m_Type(type)
{
}

qbert::LevelCube::~LevelCube()
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
