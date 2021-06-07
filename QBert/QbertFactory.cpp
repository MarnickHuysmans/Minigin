#include "QbertFactory.h"

#include "GameObject.h"
#include "LevelCubeActivator.h"
#include "Movement.h"
#include "PlayerInput.h"
#include "RenderComponent.h"
#include "Walkable.h"
#include "Level.h"
#include "Lives.h"
#include "TextComponent.h"
#include "Scene.h"

glm::vec3 qbert::QbertFactory::m_PositionOffset = {-8, -15, 1};

const std::weak_ptr<qbert::Qbert> qbert::QbertFactory::CreateQbert(dae::Scene& scene,
                                                                   const std::weak_ptr<Walkable>& startCube,
                                                                   dae::Player player,
                                                                   const std::shared_ptr<dae::Font>& font)
{
	auto qbertObject = std::make_shared<dae::GameObject>();

	auto renderComponent = std::make_shared<dae::RenderComponent>(
		player == dae::Player::Player1 ? "Sprites/Qbert.png" : "Sprites/Qbert2.png");
	auto movementComponent = std::make_shared<Movement>(startCube, m_PositionOffset, Side::Top, 0.75f, false);
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
	auto weakGameObject = startCube.lock()->GetGameObject();
	if (weakGameObject.expired())
	{
		return qbert;
	}
	weakGameObject.lock()->AddChild(qbertObject);

	//Lives
	auto go = std::make_shared<dae::GameObject>();
	auto lives = std::make_shared<Lives>(qbert->GetLives());
	go->AddComponent(lives);
	dae::Color color{};
	switch (player)
	{
	case dae::Player::Player1:
		go->GetTransform().SetPivot({0, 1});
		go->GetTransform().SetWorldPosition({10, 470, 0});
		color = {239, 86, 0};
		break;
	case dae::Player::Player2:
		go->GetTransform().SetPivot({1, 1});
		go->GetTransform().SetWorldPosition({630, 470, 0});
		color = {16, 226, 239};
		break;
	}
	auto textComponent = std::make_shared<dae::TextComponent>(font, "0", dae::Color{55, 200, 200});
	go->AddComponent(textComponent);
	scene.Add(go);

	qbert->AddObserver(lives);

	return qbert;
}
