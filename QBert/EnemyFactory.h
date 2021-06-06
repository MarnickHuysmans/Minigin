#pragma once
#include <memory>
#include <string>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

namespace dae
{
	class GameObject;
}

namespace qbert
{
	enum class Side;
	enum class Direction;
	class Movement;
	class Walkable;
	class EnemySpawner;
	
	class EnemyFactory
	{
	public:
		static const std::weak_ptr<dae::GameObject> CreateSlick(const std::shared_ptr<EnemySpawner>& enemySpawner);
		static const std::weak_ptr<dae::GameObject> CreateSam(const std::shared_ptr<EnemySpawner>& enemySpawner);

		static const std::weak_ptr<dae::GameObject> CreateUgg(const std::shared_ptr<EnemySpawner>& enemySpawner);
		static const std::weak_ptr<dae::GameObject> CreateWrongWay(const std::shared_ptr<EnemySpawner>& enemySpawner);

		static const std::weak_ptr<dae::GameObject> CreateCoily(const std::shared_ptr<EnemySpawner>& enemySpawner, bool player);

	private:
		static const std::weak_ptr<dae::GameObject> CreateSlickSam(const std::string& textureFile, const std::weak_ptr<EnemySpawner>& enemySpawner, const std::weak_ptr<Walkable>& startCube, const glm::vec3& offset);
		static const std::weak_ptr<dae::GameObject> CreateUggWrongWay(const std::string& textureFile, const std::weak_ptr<EnemySpawner>& enenmySpawner, const std::weak_ptr<Walkable>& startCube, const glm::vec3& offset, Side side);

		static glm::vec3 m_SlickOffset;
		static glm::vec3 m_SamOffset;
		static glm::vec3 m_UggOffset;
		static glm::vec3 m_WrongWayOffset;
		static glm::vec3 m_CoilyOffset;
		static glm::vec3 m_CoilyEggOffset;
	};
}
