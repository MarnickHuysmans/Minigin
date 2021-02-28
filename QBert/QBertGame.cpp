#include "pch.h"
#include "MiniginEngine.h"

namespace qbert
{
	using namespace dae;
	
	class QBertGame final : public Minigin
	{
	public:
		QBertGame() = default;
		~QBertGame() = default;

		void LoadGame() const override;

		QBertGame(const QBertGame& other) = delete;
		QBertGame(QBertGame&& other) noexcept = delete;
		QBertGame& operator=(const QBertGame& other) = delete;
		QBertGame& operator=(QBertGame&& other) noexcept = delete;
	};

	void QBertGame::LoadGame() const
	{
		Minigin::LoadGame();
	}
}

int main(int, char* [])
{
	auto qBertGame = new qbert::QBertGame();
	qBertGame->Run();
	delete qBertGame;
}
