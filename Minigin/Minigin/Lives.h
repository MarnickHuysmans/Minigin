#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae {
	class TextComponent;
}

namespace qbert
{
	class Qbert;
	using namespace dae;
	class Lives final : public Component, public Observer
	{
	public:
		void Start() override;
		void Update() override;
		void Notify(const std::string& message) override;

		explicit Lives(Qbert* qbert);
		~Lives() = default;
		Lives(const Lives& other) = delete;
		Lives(Lives&& other) noexcept = delete;
		Lives& operator=(const Lives& other) = delete;
		Lives& operator=(Lives&& other) noexcept = delete;

	private:
		TextComponent* m_TextComponent = nullptr;
		Qbert* m_Qbert = nullptr;
	};
}
