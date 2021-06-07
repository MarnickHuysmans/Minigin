#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae {
	class TextComponent;
}

namespace qbert
{
	class QbertDeleteThis;

	class LivesDeleteThis final : public dae::Component, public dae::Observer
	{
	public:
		void Start() override;
		void Update() override;
		void Notify(const std::string& message) override;

		explicit LivesDeleteThis(QbertDeleteThis* qbert);
		~LivesDeleteThis() = default;
		LivesDeleteThis(const LivesDeleteThis & other) = delete;
		LivesDeleteThis(LivesDeleteThis && other) noexcept = delete;
		LivesDeleteThis& operator=(const LivesDeleteThis & other) = delete;
		LivesDeleteThis& operator=(LivesDeleteThis && other) noexcept = delete;

	private:
		dae::TextComponent* m_TextComponent = nullptr;
		QbertDeleteThis* m_Qbert = nullptr;
	};
}
