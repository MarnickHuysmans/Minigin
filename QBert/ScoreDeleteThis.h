#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae {
	class TextComponent;
}

namespace qbert
{
	class ScoreDeleteThis final : public dae::Component, public dae::Observer
	{
	public:
		void Start() override;
		void Update() override;
		void Notify(const std::string& message) override;

		ScoreDeleteThis() = default;
		~ScoreDeleteThis() = default;
		ScoreDeleteThis(const ScoreDeleteThis& other) = delete;
		ScoreDeleteThis(ScoreDeleteThis&& other) noexcept = delete;
		ScoreDeleteThis& operator=(const ScoreDeleteThis& other) = delete;
		ScoreDeleteThis& operator=(ScoreDeleteThis&& other) noexcept = delete;

	private:
		dae::TextComponent* m_TextComponent = nullptr;
		int m_Score = 0;
	};
}
