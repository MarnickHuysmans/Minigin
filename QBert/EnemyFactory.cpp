#include "EnemyFactory.h"
#include "Level.h"
#include "EnemySpawner.h"
#include "LevelCubeActivator.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Walkable.h"
#include "LevelEnums.h"
#include "MoveDown.h"
#include "Movement.h"
#include "PlayerInput.h"
#include "SlickSam.h"

glm::vec3 qbert::EnemyFactory::m_SlickOffset = { -8, -15, 1 };
glm::vec3 qbert::EnemyFactory::m_SamOffset = { -8, -15, 1 };
glm::vec3 qbert::EnemyFactory::m_UggOffset = { -3,-3,1 };
glm::vec3 qbert::EnemyFactory::m_WrongWayOffset = { -13,-3,1 };
glm::vec3 qbert::EnemyFactory::m_CoilyOffset = { -8, -14,1 };
glm::vec3 qbert::EnemyFactory::m_CoilyEggOffset = { -8, -15,1 };

const std::weak_ptr<dae::GameObject> qbert::EnemyFactory::CreateSlick(const std::shared_ptr<qbert::EnemySpawner>& enemySpawner)
{
	auto& level = enemySpawner->GetLevel();
	if (level.expired())
	{
		return std::weak_ptr<dae::GameObject>();
	}
	return CreateSlickSam("Sprites/Slick.png", enemySpawner, level.lock()->GetTopLeftCube(), m_SlickOffset);
}

const std::weak_ptr<dae::GameObject> qbert::EnemyFactory::CreateSam(const std::shared_ptr<qbert::EnemySpawner>& enemySpawner)
{
	auto& level = enemySpawner->GetLevel();
	if (level.expired())
	{
		return std::weak_ptr<dae::GameObject>();
	}
	return CreateSlickSam("Sprites/Sam.png", enemySpawner, level.lock()->GetTopRightCube(), m_SamOffset);
}

const std::weak_ptr<dae::GameObject> qbert::EnemyFactory::CreateUgg(const std::shared_ptr<qbert::EnemySpawner>& enemySpawner)
{
	auto& level = enemySpawner->GetLevel();
	if (level.expired())
	{
		return std::weak_ptr<dae::GameObject>();
	}
	return CreateUggWrongWay("Sprites/Ugg.png", enemySpawner, level.lock()->GetBottomRightCube(), m_UggOffset, Side::Right);
}

const std::weak_ptr<dae::GameObject> qbert::EnemyFactory::CreateWrongWay(const std::shared_ptr<qbert::EnemySpawner>& enemySpawner)
{
	auto& level = enemySpawner->GetLevel();
	if (level.expired())
	{
		return std::weak_ptr<dae::GameObject>();
	}
	return CreateUggWrongWay("Sprites/WrongWay.png", enemySpawner, level.lock()->GetBottomLeftCube(), m_WrongWayOffset, Side::Left);
}

const std::weak_ptr<dae::GameObject> qbert::EnemyFactory::CreateCoily(const std::shared_ptr<EnemySpawner>& enemySpawner, bool player)
{
	auto& level = enemySpawner->GetLevel();
	if (level.expired())
	{
		return std::weak_ptr<dae::GameObject>();
	}

	bool spawnSide = rand() % 2;
	auto startCube = spawnSide ? level.lock()->GetTopLeftCube() : level.lock()->GetTopRightCube();

	if (startCube.expired())
	{
		return std::weak_ptr<dae::GameObject>();
	}

	auto gameObject = std::make_shared<dae::GameObject>();

	auto renderComponent = std::make_shared<dae::RenderComponent>("Sprites/CoilyEgg.png");
	auto movementComponent = std::make_shared<Movement>(startCube, m_CoilyEggOffset, Side::Top, 0);
	auto moveDown = std::make_shared<MoveDown>(Direction::Down , Direction::Right);
	auto enemyHit = std::make_shared<EnemyHit>(enemySpawner);
	movementComponent->AddObserver(enemyHit);

	gameObject->AddComponent(enemyHit);
	gameObject->AddComponent(renderComponent);
	gameObject->AddComponent(movementComponent);
	gameObject->AddComponent(moveDown);

	if (player)
	{
		auto playerInput = std::make_shared<PlayerInput>(dae::Player::Player2);
		gameObject->AddComponent(playerInput);
	}
	
	startCube.lock()->GetGameObject()->AddChild(gameObject);
	return gameObject;
}

const std::weak_ptr<dae::GameObject> qbert::EnemyFactory::CreateSlickSam(const std::string& textureFile, const std::weak_ptr<EnemySpawner>& enemySpawner, const std::weak_ptr<Walkable>& startCube, const glm::vec3& offset)
{
	if (startCube.expired())
	{
		return std::weak_ptr<dae::GameObject>();
	}
	auto gameObject = std::make_shared<dae::GameObject>();

	auto renderComponent = std::make_shared<dae::RenderComponent>(textureFile);
	auto movementComponent = std::make_shared<Movement>(startCube, offset, Side::Top, 0);
	auto levelCubeActivator = std::make_shared<LevelCubeActivator>(false);
	auto moveDown = std::make_shared<MoveDown>(Direction::Right, Direction::Down);
	auto slickSam = std::make_shared<SlickSam>(enemySpawner);
	movementComponent->AddObserver(slickSam);

	gameObject->AddComponent(slickSam);
	gameObject->AddComponent(renderComponent);
	gameObject->AddComponent(movementComponent);
	gameObject->AddComponent(levelCubeActivator);
	gameObject->AddComponent(moveDown);
	
	startCube.lock()->GetGameObject()->AddChild(gameObject);
	return gameObject;
}

const std::weak_ptr<dae::GameObject> qbert::EnemyFactory::CreateUggWrongWay(const std::string& textureFile, const std::weak_ptr<EnemySpawner>& enenmySpawner, const std::weak_ptr<Walkable>& startCube, const glm::vec3& offset, Side side)
{
	if (startCube.expired())
	{
		return std::weak_ptr<dae::GameObject>();
	}
	auto gameObject = std::make_shared<dae::GameObject>();

	auto renderComponent = std::make_shared<dae::RenderComponent>(textureFile);
	auto movementComponent = std::make_shared<Movement>(startCube, offset, side, 0);
	auto moveDown = std::make_shared<MoveDown>(Direction::Up, side == Side::Right ? Direction::Left : Direction::Right);
	auto enemyHit = std::make_shared<EnemyHit>(enenmySpawner, side);
	movementComponent->AddObserver(enemyHit);

	gameObject->AddComponent(enemyHit);
	gameObject->AddComponent(renderComponent);
	gameObject->AddComponent(movementComponent);
	gameObject->AddComponent(moveDown);
	
	startCube.lock()->GetGameObject()->AddChild(gameObject);
	return gameObject;
}
