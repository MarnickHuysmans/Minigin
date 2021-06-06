#pragma once
#include <Component.h>

namespace dae
{
	class TextComponent;
}

namespace qbert
{
	class Score : public dae::Component
	{
	public:
		Score();
		virtual ~Score() = default;

		void Start() override;

		void AddScore(unsigned int score);

	private:
		std::weak_ptr<dae::TextComponent> m_TextComponent;
		unsigned int m_Score;
	};
}
