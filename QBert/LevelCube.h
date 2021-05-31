#pragma once
#include <vector>

#include "RenderComponent.h"
#include "Walkable.h"

namespace qbert
{
	enum class LevelType;
	
	class LevelCube final : public Walkable
	{
	public:
		LevelCube(LevelType type, const std::weak_ptr<Level>& level, int row, int col, const glm::vec2& topMiddle = glm::vec2(0, 0), const glm::vec2& rightMiddle = glm::vec2(0, 0), const glm::vec2& leftMiddle = glm::vec2(0, 0));
		virtual ~LevelCube() = default;

		void SetTexture(std::shared_ptr<dae::Texture2D>& texture, size_t index);

		void Start() override;

		WalkableType GetWalkableType() override;

		void StepOn(bool forward = true);
		
	private:
		size_t MaxStates(LevelType type) const;
		void SetCurrentTexture();

		void StepOnStandard(bool forward);
		void StepOnCycle(bool forward);

		size_t m_State;
		size_t m_MaxStates;

		std::vector<std::shared_ptr<dae::Texture2D>> m_Textures;

		std::weak_ptr<dae::RenderComponent> m_RenderComponent;

		LevelType m_Type;
	};
}
