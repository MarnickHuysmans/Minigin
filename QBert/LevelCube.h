#pragma once
#include <vector>

#include "RenderComponent.h"
#include "Texture2D.h"
#include "Walkable.h"

namespace qbert
{
	enum class LevelType;

	class LevelCube final : public Walkable
	{
	public:
		LevelCube(LevelType type);
		virtual ~LevelCube();

		void SetTexture(std::shared_ptr<dae::Texture2D>& texture, size_t index);

	private:
		size_t MaxStates(LevelType type) const;

		size_t m_State;
		size_t m_MaxStates;

		std::vector<std::shared_ptr<dae::Texture2D>> m_Textures;

		std::weak_ptr<dae::RenderComponent> m_RenderComponent;

		LevelType m_Type;
	};
}
