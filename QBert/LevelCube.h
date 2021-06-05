#pragma once
#include <functional>
#include <vector>

#include "RenderComponent.h"
#include "Walkable.h"

namespace qbert
{
	enum class LevelType;

	class LevelCubeObserver;

	class LevelCube final : public Walkable
	{
	public:
		LevelCube(LevelType type, const std::weak_ptr<Level>& level, int row, int col, const glm::vec3& topMiddle = glm::vec3(0, 0, 0), const glm::vec3& rightMiddle = glm::vec3(0, 0, 0), const glm::vec3& leftMiddle = glm::vec3(0, 0, 0));
		virtual ~LevelCube() = default;

		void SetTexture(std::shared_ptr<dae::Texture2D>& texture, size_t index);

		void Start() override;

		WalkableType GetWalkableType() override;

		void StepOn(Movement* movement) override;

		void NextLevel() override;

		void AddObserver(const std::weak_ptr<LevelCubeObserver>& observer);

	private:
		void StepOn(bool forward = true);
		size_t MaxStates(LevelType type) const;
		void SetCurrentTexture();

		void StepOnStandard(bool forward);
		void StepOnCycle(bool forward);

		void NotifyObservers(std::function<void(LevelCubeObserver*)> observerFunction);

		size_t m_State;
		size_t m_MaxStates;

		std::vector<std::shared_ptr<dae::Texture2D>> m_Textures;

		std::vector<std::weak_ptr<LevelCubeObserver>> m_LevelCubeObservers;

		std::weak_ptr<dae::RenderComponent> m_RenderComponent;

		LevelType m_Type;
	};
}
