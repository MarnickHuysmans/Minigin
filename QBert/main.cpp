#include <vld.h>
#include "Minigin.h"
#include "QbertScenes.h"
#include "ServiceLocator.h"

class QBertGame : public dae::Minigin
{
public:

	void LoadGame() const override;
};

void QBertGame::LoadGame() const
{
	dae::SceneManager::GetInstance().CreateScene("StartScene", qbert::StartScene());
	dae::SceneManager::GetInstance().CreateScene("GameScene", qbert::GameScene());

	dae::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sound/GameStartMusic.wav");
}

int main(int, char* [])
{
	QBertGame game{};
	game.Run();
	return 0;
}
