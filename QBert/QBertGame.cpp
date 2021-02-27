#include "pch.h"
#include "MiniginEngine.h"

class QBertGame : public dae::Minigin
{
public:
	QBertGame() = default;
	~QBertGame() = default;
};

int main(int, char* [])
{
	QBertGame* qBertGame = new QBertGame();
	qBertGame->Run();
	delete qBertGame;
}
