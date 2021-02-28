#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae {
	class TextComponent;
}

namespace qbert
{
	using namespace dae;
	class Score final : public Component, public Observer
	{
	public:
		void Start() override;
		void Update() override;
		void Notify(const std::string& message) override;

		Score() = default;
		~Score() = default;
		Score(const Score& other) = delete;
		Score(Score&& other) noexcept = delete;
		Score& operator=(const Score& other) = delete;
		Score& operator=(Score&& other) noexcept = delete;

	private:
		TextComponent* m_TextComponent = nullptr;
		int m_Score = 0;
	};

}
