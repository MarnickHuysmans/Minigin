#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae {
	class TextComponent;
}

namespace qbert
{
	class Score final : public dae::Component, public dae::Observer
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
		dae::TextComponent* m_TextComponent = nullptr;
		int m_Score = 0;
	};
}
