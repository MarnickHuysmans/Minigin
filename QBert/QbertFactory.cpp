#include "QbertFactory.h"

#include "GameObject.h"
#include "LevelCubeActivator.h"
#include "Movement.h"
#include "PlayerInput.h"
#include "RenderComponent.h"
#include "Walkable.h"

glm::vec3 qbert::QbertFactory::m_PositionOffset = { -8, -15, 1 };

const std::weak_ptr<qbert::Qbert> qbert::QbertFactory::CreateQbert(const std::weak_ptr<Walkable>& startCube, dae::Player player)
{
	auto qbertObject = std::make_shared<dae::GameObject>();

	auto renderComponent = std::make_shared<dae::RenderComponent>("Sprites/Qbert.png");
	auto movementComponent = std::make_shared<Movement>(startCube, m_PositionOffset, Side::Top, 0.5f, false);
	auto levelCubeActivator = std::make_shared<LevelCubeActivator>(true);
	auto playerInput = std::make_shared<PlayerInput>(player);
	auto qbert = std::make_shared<Qbert>();
	movementComponent->AddObserver(qbert);

	qbertObject->AddComponent(renderComponent);
	qbertObject->AddComponent(movementComponent);
	qbertObject->AddComponent(levelCubeActivator);
	qbertObject->AddComponent(playerInput);
	qbertObject->AddComponent(qbert);
	if (startCube.expired())
	{
		return qbert;
	}
	startCube.lock()->GetGameObject()->AddChild(qbertObject);
	return qbert;
}
